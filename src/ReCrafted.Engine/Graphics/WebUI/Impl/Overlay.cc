#include "Overlay.h"
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/Config.h>

static ultralight::IndexType patternCW[] = { 0, 1, 3, 1, 2, 3 };
static ultralight::IndexType patternCCW[] = { 0, 3, 1, 1, 3, 2 };

Overlay::Overlay(ultralight::Renderer* renderer, GPUDriver* driver, int width, int height, int x, int y, float scale) :
    view_(renderer->CreateView(width, height, true)), width_(width), height_(height),
    x_(x), y_(y), scale_(scale), driver_(driver)
{
}

Overlay::~Overlay() {
  if (vertices_.size())
    driver_->DestroyGeometry(geometry_id_);
}

void Overlay::Draw() {
  UpdateGeometry();
  
  gpu_state_.texture_1_id = view()->render_target().texture_id;

  driver_->DrawGeometry(geometry_id_, 6, 0, gpu_state_);
}

void Overlay::KeyDown(WPARAM wparam, LPARAM lparam, bool is_system_key) {
  KeyEvent evt(KeyEvent::kType_RawKeyDown, wparam, lparam, is_system_key);
  view()->FireKeyEvent(evt);
}

void Overlay::KeyUp(WPARAM wparam, LPARAM lparam, bool is_system_key) {
  KeyEvent evt(KeyEvent::kType_KeyUp, wparam, lparam, is_system_key);
  view()->FireKeyEvent(evt);
}

void Overlay::CharPress(WPARAM wparam, LPARAM lparam, bool is_system_key) {
  KeyEvent evt(KeyEvent::kType_Char, wparam, lparam, is_system_key);
  view()->FireKeyEvent(evt);
}

void Overlay::MouseMove(int x, int y) {
  MouseEvent evt;
  evt.type = MouseEvent::kType_MouseMoved;
  evt.x = x - x_;
  evt.y = y - y_;
  evt.button = MouseEvent::kButton_None;

  view()->FireMouseEvent(evt);
}

MouseEvent::Button ToButton(MouseButton button) {
  switch (button) {
  case MouseButton::kMouseButton_Left: return MouseEvent::Button::kButton_Left;
  case MouseButton::kMouseButton_Middle: return MouseEvent::Button::kButton_Middle;
  case MouseButton::kMouseButton_Right: return MouseEvent::Button::kButton_Right;
  default: return MouseEvent::Button::kButton_None;
  }
}

void Overlay::MouseButtonDown(MouseButton button, int x, int y) {
  MouseEvent evt;
  evt.type = MouseEvent::kType_MouseDown;
  evt.x = x - x_;
  evt.y = y - y_;
  evt.button = ToButton(button);

  view()->FireMouseEvent(evt);
}

void Overlay::MouseButtonUp(MouseButton button, int x, int y) {
  MouseEvent evt;
  evt.type = MouseEvent::kType_MouseUp;
  evt.x = x - x_;
  evt.y = y - y_;
  evt.button = ToButton(button);

  view()->FireMouseEvent(evt);
}

void Overlay::MouseWheel(int delta, int x, int y) {
  if (!Contains(x, y))
    return;

  ScrollEvent evt;
  evt.delta_x = 0;
  evt.delta_y = delta;
  evt.type = ScrollEvent::kType_ScrollByPixel;

  view()->FireScrollEvent(evt);
}

void Overlay::OnFinishLoading(ultralight::View* caller)
{
}

void Overlay::UpdateGeometry() {
  bool initial_creation = false;
  if (vertices_.empty()) {
    vertices_.resize(4);
    indices_.resize(6);

    auto& config = ultralight::Platform::instance().config();
    if (config.face_winding == kFaceWinding_Clockwise)
      memcpy(indices_.data(), patternCW, sizeof(IndexType) * indices_.size());
    else
      memcpy(indices_.data(), patternCCW, sizeof(IndexType) * indices_.size());

    memset(&gpu_state_, 0, sizeof(gpu_state_));
    ultralight::Matrix identity;
    identity.SetIdentity();
    gpu_state_.transform = ConvertAffineTo4x4(identity);
    gpu_state_.enable_blend = true;
    gpu_state_.enable_texturing = true;
    gpu_state_.shader_type = kShaderType_Fill;
    gpu_state_.render_buffer_id = 0; // default render target view (screen)

    initial_creation = true;
  }

  if (!needs_update_)
    return;

  Vertex_2f_4ub_2f_2f_28f v;
  memset(&v, 0, sizeof(v));

  v.data0[0] = 1; // Fill Type: Image

  v.color[0] = 255;
  v.color[1] = 255;
  v.color[2] = 255;
  v.color[3] = 255;

  float left = (float)x_;
  float top = (float)y_;
  float right = (float)x_ + width();
  float bottom = (float)y_ + height();

  // TOP LEFT
  v.pos[0] = v.obj[0] = left;
  v.pos[1] = v.obj[1] = top;
  v.tex[0] = 0;
  v.tex[1] = 0;

  vertices_[0] = v;

  // TOP RIGHT
  v.pos[0] = v.obj[0] = right;
  v.pos[1] = v.obj[1] = top;
  v.tex[0] = 1;
  v.tex[1] = 0;

  vertices_[1] = v;

  // BOTTOM RIGHT
  v.pos[0] = v.obj[0] = right;
  v.pos[1] = v.obj[1] = bottom;
  v.tex[0] = 1;
  v.tex[1] = 1;

  vertices_[2] = v;

  // BOTTOM LEFT
  v.pos[0] = v.obj[0] = left;
  v.pos[1] = v.obj[1] = bottom;
  v.tex[0] = 0;
  v.tex[1] = 1;

  vertices_[3] = v;

  VertexBuffer vbuffer;
  vbuffer.format = kVertexBufferFormat_2f_4ub_2f_2f_28f;
  vbuffer.size = sizeof(Vertex_2f_4ub_2f_2f_28f) * vertices_.size();
  vbuffer.data = (uint8_t*)vertices_.data();

  IndexBuffer ibuffer;
  ibuffer.size = sizeof(IndexType) * indices_.size();
  ibuffer.data = (uint8_t*)indices_.data();

  if (initial_creation) {
    geometry_id_ = driver_->NextGeometryId();
    driver_->CreateGeometry(geometry_id_, vbuffer, ibuffer);
  } else {
    driver_->UpdateGeometry(geometry_id_, vbuffer, ibuffer);
  }

  needs_update_ = false;
}
