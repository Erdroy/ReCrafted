#pragma once

#include <Ultralight/platform/GPUDriver.h>
#include <Ultralight/platform/Config.h>

namespace ultralight {

class GPUContext {
public:
  virtual ~GPUContext() {}

  virtual ultralight::GPUDriver* driver() const = 0;

  virtual ultralight::FaceWinding face_winding() const = 0;
    
  virtual void BeginDrawing() = 0;

  virtual void EndDrawing() = 0;

  virtual void Resize(int width, int height) = 0;
};

}  // namespace ultralight
