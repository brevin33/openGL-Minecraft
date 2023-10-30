#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "world.h"
#include "globalSettings.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    NOTMOVING,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 15.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const float GRAVITY = -28.8f;
const float MAXACCEL = 60.0f;
const int PLAYERHEIGHT = 2;
const float jumpHeight = 1.7f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
    glm::vec3 velocity;
    glm::vec3 input;
    bool grounded;
    bool jumping;
    bool rightClicked = false;
    int activeBlock = DIRT;
    bool leftClicked = false;

    glm::vec3 getBlockposition(const glm::vec3& pos) {
        glm::vec3 blockPos;
        blockPos.x = floorf(pos.x);
        blockPos.y = ceilf(pos.y);
        blockPos.z = ceilf(pos.z);
        return blockPos;
    }


    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        input = glm::vec3(0, 0, 0);
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        velocity = glm::vec3(0.0f, 0.0f, 0.0f);
        input = glm::vec3(0, 0, 0);
        updateCameraVectors();
    }

    void breakBlock() {
        glm::vec3 currentPos = Position;
        glm::vec3 realFront = Front;
        realFront.y += 0.3f;
        for (size_t i = 0; i < 72; i++)
        {
            currentPos = 0.07f * realFront + currentPos;
            glm::vec3 currentBlockPos = getBlockposition(currentPos);
            if (world.getBlockFromWorldPos(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z).blockType >= 0) {
                world.removeBlockFromWorldPos(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z);
                break;
            }
        }
    }

    void placeBlock() {
        glm::vec3 currentPos = Position;
        glm::vec3 lastBlockPos = glm::vec3(-9999,-9999,-9999);
        glm::vec3 realFront = Front;
        realFront.y += 0.3f;
        for (size_t i = 0; i < 100; i++)
        {
            currentPos = 0.07f * realFront + currentPos;
            glm::vec3 currentBlockPos = getBlockposition(currentPos);
            if (world.getBlockFromWorldPos(currentBlockPos.x, currentBlockPos.y, currentBlockPos.z).blockType >= 0) {
                if (lastBlockPos != glm::vec3(-9999, -9999, -9999))
                    world.placeBlockFromWorldPos(lastBlockPos.x, lastBlockPos.y, lastBlockPos.z,activeBlock);
                break;
            }
            lastBlockPos = currentBlockPos;
        }
    }

    void update(float deltaTime) {
        if(leftClicked)
            breakBlock();
        if (rightClicked)
            placeBlock();
        leftClicked = false;
        rightClicked = false;
        input = input != glm::vec3(0,0,0) ? glm::normalize(input) : glm::vec3(0, 0, 0);
        glm::vec3 desiredVelocity = glm::vec3(MovementSpeed, 0, MovementSpeed) * input;
        desiredVelocity.y = velocity.y;
        velocity = MoveTowards(velocity,desiredVelocity,MAXACCEL);
        velocity.y += GRAVITY * deltaTime;
        if (jumping && grounded) {
            velocity.y = glm::sqrt(2.0f * -GRAVITY * jumpHeight);
        }
        glm::vec3 newPosition = Position + ( velocity * deltaTime);
        glm::vec3 lastPos = Position;
        glm::vec3 blockPos = getBlockposition(Position);
        glm::vec3 newBlockPos = getBlockposition(newPosition);
        if (blockPos.x == newBlockPos.x || world.getBlockFromWorldPos(newBlockPos.x, blockPos.y - PLAYERHEIGHT, blockPos.z).blockType < 0)// chuck for block at pos
        {
            Position.x = newPosition.x;
        }
        if (blockPos.y == newBlockPos.y || world.getBlockFromWorldPos(blockPos.x, newBlockPos.y - PLAYERHEIGHT, blockPos.z).blockType < 0)// chuck for block at pos
        {
            grounded = false;
            Position.y = newPosition.y;
        }
        else {
            grounded = true;
        }
        if (blockPos.z == newBlockPos.z || world.getBlockFromWorldPos(blockPos.x, blockPos.y - PLAYERHEIGHT, newBlockPos.z).blockType < 0)// chuck for block at pos
        {
            Position.z = newPosition.z;
        }
        jumping = false;

        input = glm::vec3(0,0,0);
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(glm::vec3(Position.x, Position.y-0.4f, Position.z), Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        if (direction == FORWARD)
            input += Front;
        if (direction == BACKWARD)
            input -= Front;
        if (direction == LEFT)
            input -= Right;
        if (direction == RIGHT)
            input += Right;
        input.y = 0;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
#endif