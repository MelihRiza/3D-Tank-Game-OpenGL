#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 2, 5);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 5;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            this->center = center;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void MoveForward(float distance, glm::vec3 direction)
        {
            // Translates the camera using the `dir` vector computed from
            // `forward`. Movement will always keep the camera at the same
            // height. For example, if you rotate your head up/down, and then
            // walk forward, then you will still keep the same relative
            // distance (height) to the ground!

            position += direction * distance;
            this->center += direction * distance;
        }

        void TranslateForward(float distance)
        {
            // TODO(student): Translate the camera using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, forward.y, forward.z));
            position += dir * distance;
        }

        void TranslateUpward(float distance)
        {
            // TODO(student): Translate the camera using the `up` vector.
            glm::vec3 dir = glm::vec3(0, up.y, 0);
            position += dir * distance;
        }

        void TranslateRight(float distance)
        {
            // TODO(student): See instructions below. Read the entire thing!
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.
            glm::vec3 RightProjected = glm::normalize(glm::cross(forward, up));

            glm::vec3 dir = glm::vec3(RightProjected[0], 0, RightProjected[2]);
            position += dir * distance;

        }

        void RotateFirstPerson_OX(float angle)
        {
            // TODO(student): Compute the new `forward` and `up` vectors.
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            up = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, right) * glm::vec4(up, 1)));
            forward = glm::normalize(glm::cross(up, right));

           
        }

        void RotateFirstPerson_OY(float angle)
        {
            // TODO(student): Compute the new `forward`, `up` and `right`
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!
            forward = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1)));
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));

        }


        void RotateFirstPerson_OZ(float angle)
        {
            // TODO(student): Compute the new `right` and `up`. This time,
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!
            right = glm::normalize(glm::vec3(glm::rotate(glm::mat4(1), angle, forward) * glm::vec4(right, 1)));
            up = glm::normalize(glm::cross(right, forward));
        }
        
        void RotateThirdPerson_OX(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OX axis. Use `distanceToTarget` as translation distance.


            glm::vec3 targetPos = center;
            glm::vec3 currentPosition = position;
            glm::vec3 relativePosition = targetPos - position;

            TranslateForward(glm::distance(relativePosition, glm::vec3(0, 0, 0)));
            RotateFirstPerson_OX(angle);
            TranslateForward(-glm::distance(relativePosition, glm::vec3(0, 0, 0)));
        }

        void RotateThirdPerson_OY(float angle, glm::vec3 current_tank_center)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OY axis.


            glm::vec3 targetPos = center;
            glm::vec3 currentPosition = position;
            glm::vec3 relativePosition = targetPos - position;

            TranslateForward(glm::distance(relativePosition, glm::vec3(0, 0, 0)));
            RotateFirstPerson_OY(angle);
            TranslateForward(-glm::distance(relativePosition, glm::vec3(0, 0, 0)));

        }

        void RotateThirdPerson_OZ(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OZ axis.

            glm::vec3 targetPosition = GetTargetPosition();
            glm::vec3 relativePos = position - targetPosition;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, forward);

            glm::vec3 rotatedPos = glm::vec3(rotationMatrix * glm::vec4(relativePos, 1.0f));
            position = GetTargetPosition() + rotatedPos;

            forward = glm::normalize(targetPosition - position);
            right = glm::normalize(glm::cross(forward, up));
            up = glm::normalize(glm::cross(right, forward));
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
        glm::vec3 center; // added by me
    };
}   // namespace implemented
