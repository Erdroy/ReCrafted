#pragma once

#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>

namespace ultralight {

class GPUContext {
public:
  virtual ~GPUContext() {}

  static std::unique_ptr<GPUContext> Create(bool enable_vsync);

  virtual ultralight::GPUDriver* driver() const = 0;

  virtual ultralight::FaceWinding face_winding() const = 0;
    
  virtual void BeginDrawing() = 0;

  virtual void EndDrawing() = 0;

  // Should only be called during drawing phase
  virtual void PresentFrame() = 0;

  virtual void Resize(int width, int height) = 0;
};

}  // namespace ultralight
