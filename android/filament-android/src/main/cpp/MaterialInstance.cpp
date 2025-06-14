/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <jni.h>

#include <filament/MaterialInstance.h>
#include <filament/Texture.h>
#include <filament/TextureSampler.h>

#include <math/mat3.h>
#include <math/mat4.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>

using namespace filament;
using namespace filament::math;

enum BooleanElement {
    BOOL,
    BOOL2,
    BOOL3,
    BOOL4
};

enum IntElement {
    INT,
    INT2,
    INT3,
    INT4
};

enum FloatElement {
    FLOAT,
    FLOAT2,
    FLOAT3,
    FLOAT4,
    MAT3,
    MAT4
};

template<typename T>
static void setParameter(JNIEnv* env, jlong nativeMaterialInstance, jstring name_, T v) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    const char *name = env->GetStringUTFChars(name_, 0);
    instance->setParameter(name, v);
    env->ReleaseStringUTFChars(name_, name);
}

template<typename T>
static void setConstant(JNIEnv* env, jlong nativeMaterialInstance, jstring name_, T v) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    const char *name = env->GetStringUTFChars(name_, 0);
    instance->setConstant(name, v);
    env->ReleaseStringUTFChars(name_, name);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterBool(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jboolean x) {
    setParameter(env, nativeMaterialInstance, name_, bool(x));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterBool2(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jboolean x, jboolean y) {
    setParameter(env, nativeMaterialInstance, name_, bool2{x, y});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterBool3(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jboolean x, jboolean y, jboolean z) {
    setParameter(env, nativeMaterialInstance, name_, bool3{x, y, z});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterBool4(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_,
        jboolean x, jboolean y, jboolean z, jboolean w) {
    setParameter(env, nativeMaterialInstance, name_, bool4{x, y, z, w});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterInt(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint x) {
    setParameter(env, nativeMaterialInstance, name_, int32_t(x));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterInt2(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint x, jint y) {
    setParameter(env, nativeMaterialInstance, name_, int2{x, y});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterInt3(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint x, jint y, jint z) {
    setParameter(env, nativeMaterialInstance, name_, int3{x, y, z});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterInt4(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_,
        jint x, jint y, jint z, jint w) {
    setParameter(env, nativeMaterialInstance, name_, int4{x, y, z, w});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterFloat(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jfloat x) {
    setParameter(env, nativeMaterialInstance, name_, float(x));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterFloat2(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jfloat x, jfloat y) {
    setParameter(env, nativeMaterialInstance, name_, float2{x, y});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterFloat3(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jfloat x, jfloat y, jfloat z) {
    setParameter(env, nativeMaterialInstance, name_, float3{x, y, z});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterFloat4(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_,
        jfloat x, jfloat y, jfloat z, jfloat w) {
    setParameter(env, nativeMaterialInstance, name_, float4{x, y, z, w});
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetBooleanParameterArray(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint element,
        jbooleanArray v_, jint offset, jint count) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;

    const char* name = env->GetStringUTFChars(name_, 0);
    jboolean* v = env->GetBooleanArrayElements(v_, NULL);

    // NOTE: In C++, bool has an implementation-defined size. Here we assume
    // it has the same size as jboolean, which is 1 byte.

    switch ((BooleanElement) element) {
        case BOOL:
            instance->setParameter(name, ((const bool*) v) + offset, count);
            break;
        case BOOL2:
            instance->setParameter(name, ((const bool2*) v) + offset, count);
            break;
        case BOOL3:
            instance->setParameter(name, ((const bool3*) v) + offset, count);
            break;
        case BOOL4:
            instance->setParameter(name, ((const bool4*) v) + offset, count);
            break;
    }

    env->ReleaseBooleanArrayElements(v_, v, 0);

    env->ReleaseStringUTFChars(name_, name);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetIntParameterArray(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint element,
        jintArray v_, jint offset, jint count) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;

    const char* name = env->GetStringUTFChars(name_, 0);
    jint* v = env->GetIntArrayElements(v_, NULL);

    switch ((IntElement) element) {
        case INT:
            instance->setParameter(name, ((const int32_t*) v) + offset, count);
            break;
        case INT2:
            instance->setParameter(name, ((const int2*) v) + offset, count);
            break;
        case INT3:
            instance->setParameter(name, ((const int3*) v) + offset, count);
            break;
        case INT4:
            instance->setParameter(name, ((const int4*) v) + offset, count);
            break;
    }

    env->ReleaseIntArrayElements(v_, v, JNI_ABORT);

    env->ReleaseStringUTFChars(name_, name);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetFloatParameterArray(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jint element,
        jfloatArray v_, jint offset, jint count) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;

    const char* name = env->GetStringUTFChars(name_, 0);
    jfloat* v = env->GetFloatArrayElements(v_, NULL);

    switch ((FloatElement) element) {
        case FLOAT:
            instance->setParameter(name, ((const float*) v) + offset, count);
            break;
        case FLOAT2:
            instance->setParameter(name, ((const float2*) v) + offset, count);
            break;
        case FLOAT3:
            instance->setParameter(name, ((const float3*) v) + offset, count);
            break;
        case FLOAT4:
            instance->setParameter(name, ((const float4*) v) + offset, count);
            break;
        case MAT3:
            instance->setParameter(name, ((const mat3f*) v) + offset, count);
            break;
        case MAT4:
            instance->setParameter(name, ((const mat4f*) v) + offset, count);
            break;
    }

    env->ReleaseFloatArrayElements(v_, v, 0);

    env->ReleaseStringUTFChars(name_, name);
}

// defined in TextureSampler.cpp
namespace filament::JniUtils {
    TextureSampler from_long(jlong params) noexcept;
} // TextureSamplerJniUtils

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetParameterTexture(
        JNIEnv *env, jclass, jlong nativeMaterialInstance, jstring name_,
        jlong nativeTexture, jlong sampler_) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    Texture* texture = (Texture*) nativeTexture;

    const char *name = env->GetStringUTFChars(name_, 0);
    instance->setParameter(name, texture, JniUtils::from_long(sampler_));
    env->ReleaseStringUTFChars(name_, name);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetConstantBool(JNIEnv *env, jclass,
        jlong nativeMaterialInstance, jstring name_, jboolean x) {
    setConstant(env, nativeMaterialInstance, name_, bool(x));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetScissor(
        JNIEnv *env, jclass, jlong nativeMaterialInstance, jint left,
        jint bottom, jint width, jint height) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setScissor(left, bottom, width, height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nUnsetScissor(
        JNIEnv *env, jclass, jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->unsetScissor();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetPolygonOffset(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jfloat scale, jfloat constant) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setPolygonOffset(scale, constant);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetMaskThreshold(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jfloat threshold) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setMaskThreshold(threshold);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetSpecularAntiAliasingVariance(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jfloat variance) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setSpecularAntiAliasingVariance(variance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetSpecularAntiAliasingThreshold(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jfloat threshold) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setSpecularAntiAliasingThreshold(threshold);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetDoubleSided(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jboolean doubleSided) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setDoubleSided(doubleSided);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetCullingMode(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jlong cullingMode) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setCullingMode((MaterialInstance::CullingMode) cullingMode);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetCullingModeSeparate(JNIEnv*, jclass,
        jlong nativeMaterialInstance,
        jlong colorPassCullingMode, jlong shadowPassCullingMode) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setCullingMode(
            (MaterialInstance::CullingMode) colorPassCullingMode,
            (MaterialInstance::CullingMode) shadowPassCullingMode);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetColorWrite(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jboolean enable) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setColorWrite(enable);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetDepthWrite(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jboolean enable) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setDepthWrite(enable);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilWrite(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jboolean enable) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilWrite(enable);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetDepthCulling(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jboolean enable) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setDepthCulling(enable);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetDepthFunc(JNIEnv*,
        jclass, jlong nativeMaterialInstance, jlong function) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setDepthFunc(static_cast<MaterialInstance::DepthFunc>(function));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilCompareFunction(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jlong function, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilCompareFunction(
            static_cast<MaterialInstance::StencilCompareFunc>(function),
            static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilOpStencilFail(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jlong op, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilOpStencilFail(
            static_cast<MaterialInstance::StencilOperation>(op),
            static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilOpDepthFail(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jlong op, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilOpDepthFail(
            static_cast<MaterialInstance::StencilOperation>(op),
            static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilOpDepthStencilPass(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jlong op, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilOpDepthStencilPass(
            static_cast<MaterialInstance::StencilOperation>(op),
            static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilReferenceValue(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jint value, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilReferenceValue(value, static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilReadMask(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jint readMask, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilReadMask(readMask, static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_google_android_filament_MaterialInstance_nSetStencilWriteMask(JNIEnv*, jclass,
        jlong nativeMaterialInstance, jint writeMask, jlong face) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    instance->setStencilWriteMask(writeMask, static_cast<MaterialInstance::StencilFace>(face));
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_google_android_filament_MaterialInstance_nGetName(JNIEnv* env, jclass,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    return env->NewStringUTF(instance->getName());
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_google_android_filament_MaterialInstance_nGetMaterial(JNIEnv* env, jclass,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*) nativeMaterialInstance;
    return (jlong) instance->getMaterial();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_google_android_filament_MaterialInstance_nDuplicate(JNIEnv* env, jclass clazz,
        jlong otherNativeMaterialInstance, jstring name) {
    MaterialInstance* other = (MaterialInstance*) otherNativeMaterialInstance;
    const char *cstr = nullptr;
    if (name != nullptr) {
        cstr = env->GetStringUTFChars(name, nullptr);
    }
    MaterialInstance* mi = MaterialInstance::duplicate(other, cstr);
    if (name != nullptr) {
        env->ReleaseStringUTFChars(name, cstr);
    }
    return (jlong)mi;
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_google_android_filament_MaterialInstance_nGetMaskThreshold(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->getMaskThreshold();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_google_android_filament_MaterialInstance_nGetSpecularAntiAliasingVariance(JNIEnv* env,
        jclass clazz, jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->getSpecularAntiAliasingVariance();
}

extern "C"
JNIEXPORT jfloat JNICALL
Java_com_google_android_filament_MaterialInstance_nGetSpecularAntiAliasingThreshold(JNIEnv* env,
        jclass clazz, jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->getSpecularAntiAliasingThreshold();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_google_android_filament_MaterialInstance_nIsDoubleSided(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->isDoubleSided();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_google_android_filament_MaterialInstance_nGetCullingMode(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return (jint)instance->getCullingMode();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_google_android_filament_MaterialInstance_nGetShadowCullingMode(JNIEnv* env, jclass,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return (jint)instance->getShadowCullingMode();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_google_android_filament_MaterialInstance_nIsColorWriteEnabled(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->isColorWriteEnabled();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_google_android_filament_MaterialInstance_nIsDepthWriteEnabled(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->isDepthWriteEnabled();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_google_android_filament_MaterialInstance_nIsStencilWriteEnabled(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->isStencilWriteEnabled();
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_google_android_filament_MaterialInstance_nIsDepthCullingEnabled(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return instance->isDepthCullingEnabled();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_google_android_filament_MaterialInstance_nGetDepthFunc(JNIEnv* env, jclass clazz,
        jlong nativeMaterialInstance) {
    MaterialInstance* instance = (MaterialInstance*)nativeMaterialInstance;
    return (jint)instance->getDepthFunc();
}
