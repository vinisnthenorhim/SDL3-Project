#pragma once

#include <renderer/raii/_raii.h>
#include <struct/struct.h>

class Visible
{
  private:
    TransferBuffer existTransfer        ;
    TransferBuffer imageIdTransfer      ;
    TransferBuffer transparencyTransfer ;

    Buffer existBuffer            ;
    Buffer imageIdBuffer          ;
    Buffer transparencyBuffer     ;

    std::vector<bool>  *dirtyImage    ;
    const std::vector<bool>  *exist         ;
    std::vector<int>  existUint8    ;
    const std::vector<int>   *imageId       ;
    const std::vector<float> *transparency  ;

  public:
    void update( SDL_GPUDevice* device, SDL_GPUCopyPass* copypass );
    void loadData( ObjectDataRO objectDataRO, DirtyData dirtyData );
    void bindG( SDL_GPURenderPass* pass, uint32_t slot, uint32_t fragslot );
    void release();
};