/*

 pxCore Copyright 2005-2017 John Robinson

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

// pxImage.h

#ifndef PX_IMAGE_H
#define PX_IMAGE_H

#include "pxContext.h"
#include "rtMutex.h"
#include "pxTexture.h"
//#include "pxTextureCacheObject.h"
#include "pxResource.h"
#include "pxRenderTexture.h"

class pxImage: public pxObject, pxResourceListener 
{
public:
  rtDeclareObject(pxImage, pxObject);
  rtProperty(url, url, setUrl, rtString);
  rtProperty(stretchX, stretchX, setStretchX, int32_t);
  rtProperty(stretchY, stretchY, setStretchY, int32_t);
  rtProperty(resource, resource, setResource, rtObjectRef);
  rtProperty(texture, texture, setTexture, rtObjectRef);
  rtProperty(uvs, uvs, setUvs , rtObjectRef);

  pxImage(pxScene2d* scene) : pxObject(scene), mStretchX(pxConstantsStretch::NONE), mStretchY(pxConstantsStretch::NONE), mUvs(NULL), 
    imageLoaded(false), mListenerAdded(false)
  { 
    mw = -1;
    mh = -1;
    mResource = pxImageManager::getImage("");
  }

  virtual ~pxImage();

  virtual void update(double t) { pxObject::update(t);}
  virtual void onInit();
  virtual void sendPromise();
  virtual void createNewPromise() { rtLogDebug("pxImage ignoring createNewPromise\n"); }
  
  rtError url(rtString& s) const;
  rtError setUrl(const char* s);
  
  rtError stretchX(int32_t& v) const { v = (int32_t)mStretchX; return RT_OK; }
  rtError setStretchX(int32_t v);

  rtError stretchY(int32_t& v) const { v = (int32_t)mStretchY; return RT_OK; }
  rtError setStretchY(int32_t v);
  
  rtError resource(rtObjectRef& o) const { /*rtLogDebug("!!!!!!!!!!!!!!!!!!!!!!!pxImage getResource\n");*/o = mResource; return RT_OK; }
  rtError setResource(rtObjectRef o);

  rtError texture(rtObjectRef& t) const 
  {
    t = mTexture;
    return RT_OK;
  }

  rtError setTexture(rtObjectRef texture)
  {
    mTexture = texture;
    return RT_OK;
  }

  rtError uvs(rtObjectRef& /*uvs*/) const { return RT_OK; }

  rtError setUvs(rtObjectRef uvs) 
  {
    if (mUvs != NULL)
    {
      free(mUvs);
    }
    uint32_t len = uvs.get<uint32_t>("length"); 
    if(len != 8)
    {
      rtLogError("uvs length must be 8");
      return RT_ERROR;
    }
    mUvs = (float*)malloc(sizeof(float) * 8);
    for (uint32_t i = 0; i < len; i++) 
    {
      mUvs[i] = uvs.get<float>(i);
    }
    return RT_OK;
  }

  virtual void resourceReady(rtString readyResolution);
  //virtual bool onTextureReady(pxTextureCacheObject* textureCacheObject) {return true;}
  // !CLF: To Do: These names are terrible... find better ones!
  virtual float getOnscreenWidth();
  virtual float getOnscreenHeight();
  virtual void dispose();
  void checkStretchX();
  void checkStretchY();

  pxRenderTexture * getRenderTexture()
  {
    if (mTexture.getPtr() != NULL)
    {
      pxObject* t = (pxObject*) mTexture.getPtr();
      pxRenderTexture* renderTexture = dynamic_cast<pxRenderTexture*>(t);
      return renderTexture;
    }
    return NULL;
  }

protected:
  virtual void draw();
  
  virtual void drawWithImage();
  virtual void drawWithTexture(pxTextureRef texture);

  void loadImage(rtString Url);
  inline rtImageResource* getImageResource() const { return (rtImageResource*)mResource.getPtr(); }

  pxConstantsStretch::constants mStretchX;
  pxConstantsStretch::constants mStretchY;
  rtObjectRef mResource;
  rtObjectRef mTexture;
  
  float* mUvs;
  
  bool imageLoaded;
  bool mListenerAdded;
};

#endif
