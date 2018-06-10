#pragma once
#include <Ultralight/View.h>
#include <Ultralight/Renderer.h>
#include <Ultralight/platform/GPUDriver.h>
#include <vector>

using namespace ultralight;

enum MouseButton {
    kMouseButton_Left,
    kMouseButton_Middle,
    kMouseButton_Right,
};

/**
 * D3D Screen Overlay that wraps a View, useful for UI purposes.
 */
class Overlay {
public:
  Overlay(ultralight::Renderer* renderer, GPUDriver* driver, int width, int height, int x, int y, float scale);
  virtual ~Overlay();

  ultralight::Ref<View> view() { return view_; }

  int width() const { return width_; }
  int height() const { return height_; }
  int x() const { return x_; }
  int y() const { return y_; }

  void MoveTo(int x, int y) { x_ = x; y_ = y; needs_update_ = true; }
  void MoveBy(int dx, int dy) { x_ += dx; y_ += dy; needs_update_ = true; }

  virtual void Draw();

  virtual void KeyDown(WPARAM wparam, LPARAM lparam, bool is_system_key);
  virtual void KeyUp(WPARAM wparam, LPARAM lparam, bool is_system_key);
  virtual void CharPress(WPARAM wparam, LPARAM lparam, bool is_system_key);
  virtual void MouseMove(int x, int y);
  virtual void MouseButtonDown(MouseButton button, int x, int y);
  virtual void MouseButtonUp(MouseButton button, int x, int y);
  virtual void MouseWheel(int delta, int x, int y);

protected:
  void UpdateGeometry();
  bool Contains(int x, int y) const { return x >= x_ && y >= y_ && x < x_ + width_ && y < y_ + height_; }

  int width_;
  int height_;
  int x_;
  int y_;
  float scale_;
  ultralight::Ref<View> view_;
  GPUDriver* driver_;
  std::vector<Vertex_2f_4ub_2f_2f_28f> vertices_;
  std::vector<IndexType> indices_;
  bool needs_update_ = true;
  uint32_t geometry_id_ = 0;
  GPUState gpu_state_;
};