## Citro3d Graphisms dissection
(Using [3ds-examples] as dissection material.)

#### Lcd Init
```cpp
gfxInitDefault(); // (libctru) Inits the LCD framebuffers with default params.
gfxSet3D(false); // Disable 3D.

// Setting up the Render buffer for both screens.
// (Citro3d)
C3D_RenderBuf rbTop, rbBot;
```
### Render buffers
#### Init
```cpp
// ???I suppose that's probably for the 3D stuff.
#ifndef EXTENDED_TOPSCR_RESOLUTION
#define TOPSCR_WIDTH 240
#define TOPSCR_COPYFLAG 0x00001000
#else
#define TOPSCR_WIDTH (240*2)
#define TOPSCR_COPYFLAG 0x01001000
#endif

// Note : supports GPU_RB_RGB565 format
// Why 400 of height? I don't have a frigging clue
// Actually , a screen is 400*240, so the lib inits them by height,width. Welp.
// IIRC the screen controller is rotated, hence the inversion.
C3D_RenderBufInit(&rbTop, TOPSCR_WIDTH, 400, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
C3D_RenderBufInit(&rbBot, 240, 320, GPU_RB_RGBA8, GPU_RB_DEPTH24_STENCIL8);
rbTop.clearColor = CLEAR_COLOR;
rbBot.clearColor = CLEAR_COLOR;

```
#### Add VBO to buffer
```cpp
// Configure buffers
C3D_BufInfo* bufInfo = C3D_GetBufInfo();
BufInfo_Init(bufInfo);
BufInfo_Add(bufInfo, myVbo, sizeof(vertex_t), 3, 0x210);
```
#### Clean
```cpp
// Clear renderbuffers
C3D_RenderBufClear(&rbTop);
C3D_RenderBufClear(&rbBot);
```

### Graphics initialisation
#### Citro3D init
```cpp
C3D_Init(C3D_DEFAULT_CMDBUF_SIZE); // Inits C3D. Nuff said.
```

#### Matrix binding/setting up
```cpp
// Ahah, I know this one! Projection and ModelView matrices.
C3D_MtxStack projMtx, mdlvMtx;
MtxStack_Init(&projMtx);
// I don't have a clue what VSH_FVEC_ or VSH_ULEN_ do...
MtxStack_Bind(&projMtx, GPU_VERTEX_SHADER, VSH_FVEC_projMtx, VSH_ULEN_projMtx);
MtxStack_Init(&mdlvMtx);
MtxStack_Bind(&mdlvMtx, GPU_VERTEX_SHADER, VSH_FVEC_mdlvMtx, VSH_ULEN_mdlvMtx);
```
Note : Found somewhere else:
```cpp
// Compute the projection matrix
Mtx_OrthoTilt(&projection, 0.0, 400.0, 0.0, 240.0, 0.0, 1.0);
```
I suppose that bind must be for multiple screen or stuff.

### Texture
#### Init and upload
```cpp
C3D_Tex myTex;
// Load the texture and bind it to the first texture unit
C3D_TexInit(&myTex, 64, 64, GPU_RGBA8);
// Uploads the grass binary data to the text?
C3D_TexUpload(&myTex, grass_bin);
```

### Shaders
#### Parse compiled shaders
```cpp
DVLB_s *pVsh, *pGsh;
pVsh = DVLB_ParseFile((u32*)test_vsh_shbin, test_vsh_shbin_size);
pGsh = DVLB_ParseFile((u32*)test_gsh_shbin, test_gsh_shbin_size);
```
#### Init shader
```cpp
shaderProgram_s shader;
shaderProgramInit(&shader);
shaderProgramSetVsh(&shader, &pVsh->DVLE[0]);
// The last value is a stride value but why 3*5? Here's the solution
// The vertex struct gputest uses pushes vertex color to index 5
shaderProgramSetGsh(&shader, &pGsh->DVLE[0], 3*5);
```

#### Bind shader
```cpp
C3D_BindProgram(&shader);
```

#### Setitng shader program attributes
```cpp
// Prolly get the current shader program's attributes
C3D_AttrInfo* attrInfo = C3D_GetAttrInfo();
AttrInfo_Init(attrInfo);
// Attributes are only linked to gputest's way to store them.
AttrInfo_AddLoader(attrInfo, 0, GPU_FLOAT, 3); // position (xyz)
AttrInfo_AddLoader(attrInfo, 1, GPU_FLOAT, 2); // texcoord (uv)
AttrInfo_AddLoader(attrInfo, 2, GPU_FLOAT, 3); // vertex color (rgb)
```

### VBO
#### Init and set data
```cpp
// static const vertex_t vertex_list[] = {...};
// Configure VBO
myVbo = linearAlloc(sizeof(vertex_list));
// Looks like the VBO allows for direct access.
memcpy(myVbo, vertex_list, sizeof(vertex_list));
```

[3ds-examples]: https://github.com/devkitPro/3ds-examples
