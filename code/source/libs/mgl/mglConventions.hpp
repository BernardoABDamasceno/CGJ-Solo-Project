////////////////////////////////////////////////////////////////////////////////
//
// Shader Naming Conventions
//
// Copyright (c)2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CONVENTIONS_HPP
#define MGL_CONVENTIONS_HPP

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

const char MODEL_MATRIX[] = "ModelMatrix";
const char NORMAL_MATRIX[] = "NormalMatrix";
const char VIEW_MATRIX[] = "ViewMatrix";
const char PROJECTION_MATRIX[] = "ProjectionMatrix";
const char TEXTURE_MATRIX[] = "TextureMatrix";
const char CAMERA_BLOCK[] = "Camera";

const char POSITION_ATTRIBUTE[] = "inPosition";
const char NORMAL_ATTRIBUTE[] = "inNormal";
const char TEXCOORD_ATTRIBUTE[] = "inTexcoord";
const char TANGENT_ATTRIBUTE[] = "inTangent";
const char BITANGENT_ATTRIBUTE[] = "inBitangent";
const char COLOR_ATTRIBUTE[] = "inColor";

const char NEAR_UNIFORM[] = "near";
const char FAR_UNIFORM[] = "far";

const glm::vec3 RED(1.0f, 0.0f, 0.0f); // red
const glm::vec3 GREEN(0.0f, 1.0f, 0.0f); // green
const glm::vec3 BLUE(0.0f, 0.0f, 1.0f); // blue
const glm::vec3 YELLOW(1.0f, 1.0f, 0.0f); // yellow
const glm::vec3 MAGENTA(1.0f, 0.0f, 1.0f); // magenta
const glm::vec3 CYAN(0.0f, 1.0f, 1.0f); // cyan
const glm::vec3 WHITE(1.0f, 1.0f, 1.0f);  // white

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_CONVENTIONS_HPP */
