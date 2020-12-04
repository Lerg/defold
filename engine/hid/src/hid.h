// Copyright 2020 The Defold Foundation
// Licensed under the Defold License version 1.0 (the "License"); you may not use
// this file except in compliance with the License.
//
// You may obtain a copy of the License, together with FAQs at
// https://www.defold.com/license
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#ifndef DM_HID_H
#define DM_HID_H

// Winuser.h defines MAX_TOUCH_COUNT to 256, which clashes with dmHID::MAX_TOUCH_COUNT
// https://msdn.microsoft.com/en-us/library/windows/desktop/hh802879(v=vs.85).aspx
#if defined(_WIN32)
#undef MAX_TOUCH_COUNT
#endif

#include <stdint.h>

#include <dmsdk/hid/hid.h>

namespace dmHID
{
    /** max number of characters
     * @constant
     * @name dmHID::MAX_CHAR_COUNT
     */
    const uint32_t MAX_CHAR_COUNT = 256;

    /// Constant that defines invalid context handles
    const HContext INVALID_CONTEXT = 0;

    enum KeyboardType
    {
        KEYBOARD_TYPE_DEFAULT,
        KEYBOARD_TYPE_NUMBER_PAD,
        KEYBOARD_TYPE_EMAIL,
        KEYBOARD_TYPE_PASSWORD,
    };

    struct TextPacket
    {
        char     m_Text[MAX_CHAR_COUNT];
        uint32_t m_Size;
    };

    struct MarkedTextPacket
    {
        char     m_Text[MAX_CHAR_COUNT];
        uint32_t m_Size;
        uint32_t m_HasText : 1;
    };

    /**
     * Data for a single touch, e.g. finger
     */
    struct Touch
    {
        /// Single-click, double, etc
        int32_t m_TapCount;
        /// Begin, end, etc
        Phase   m_Phase;
        /// Current x
        int32_t m_X;
        /// Current y
        int32_t m_Y;
        /// Current x, in screen space
        int32_t m_ScreenX;
        /// Current y, in screen space
        int32_t m_ScreenY;
        /// Current dx
        int32_t m_DX;
        /// Current dy
        int32_t m_DY;
        /// Current dx, in screen space
        int32_t m_ScreenDX;
        /// Current dy, in screen space
        int32_t m_ScreenDY;
        /// Touch id
        int32_t m_Id;
    };

    /**
     * Data for the current touch-state
     */
    struct TouchDevicePacket
    {
        Touch    m_Touches[MAX_TOUCH_COUNT];
        uint32_t m_TouchCount;
    };

    struct AccelerationPacket
    {
        float m_X, m_Y, m_Z;
    };

    typedef void (* DMHIDGamepadFunc)(uint32_t, bool, void*);

    /// parameters to be passed to NewContext
    struct NewContextParams
    {
        NewContextParams();

        /// if mouse input should be ignored
        uint32_t m_IgnoreMouse : 1;
        /// if keyboard input should be ignored
        uint32_t m_IgnoreKeyboard : 1;
        /// if gamepad input should be ignored
        uint32_t m_IgnoreGamepads : 1;
        /// if touch device input should be ignored
        uint32_t m_IgnoreTouchDevice : 1;
        /// if acceleration input should be ignored
        uint32_t m_IgnoreAcceleration : 1;
        /// if mouse wheel scroll direction should be flipped (see DEF-2450)
        uint32_t m_FlipScrollDirection : 1;

        DMHIDGamepadFunc m_GamepadConnectivityCallback;

    };

    /**
     * Creates a new hid context.
     *
     * @params parameters to use for the new context
     * @return a new hid context, or INVALID_CONTEXT if it could not be created
     */
    HContext NewContext(const NewContextParams& params);

    /**
     * Deletes a hid context.
     *
     * @param context context to be deleted
     */
    void DeleteContext(HContext context);

    /**
     * Set user data that will be passed along to the gamepad connectivity callback.
     *
     * @params context context for which the userdata should be set
     * @params userdata userdata that should be passed along to callback
     */
    void SetGamepadFuncUserdata(HContext context, void* userdata);

    /**
     * Initializes a hid context.
     *
     * @param context context to initialize
     * @return if the context was successfully initialized or not
     */
    bool Init(HContext context);

    /**
     * Finalizes a hid context.
     *
     * @param context context to finalize
     */
    void Final(HContext context);

    /**
     * Updates a hid context by polling input from the connected hid devices.
     *
     * @param context the context to poll from
     */
    void Update(HContext context);

    /**
     * Retrieves the number of buttons on a given gamepad.
     *
     * @param gamepad gamepad handle
     * @return the number of buttons
     */
    uint32_t GetGamepadButtonCount(HGamepad gamepad);

    /**
     * Retrieves the number of axis on a given gamepad.
     *
     * @param gamepad gamepad handle
     * @return the number of axis
     */
    uint32_t GetGamepadAxisCount(HGamepad gamepad);

    /**
     * Retrieves the platform-specific device name of a given gamepad.
     *
     * @param gamepad gamepad handle
     * @param a pointer to the device name, or 0x0 if not specified
     */
    void GetGamepadDeviceName(HGamepad gamepad, const char** out_device_name);

    /**
     * Check if a keyboard is connected.
     *
     * @name IsKeyboardConnected
     * @param context context in which to search
     * @return If a keyboard is connected or not
     */
    bool IsKeyboardConnected(HKeyboard keyboard);

    /**
     * Check if a mouse is connected.
     *
     * @name IsMouseConnected
     * @param context context in which to search
     * @return If a mouse is connected or not
     */
    bool IsMouseConnected(HMouse mouse);

    /**
     * Check if the supplied gamepad is connected or not.
     *
     * @name IsGamepadConnected
     * @param gamepad Handle to gamepad
     * @return If the gamepad is connected or not
     */
    bool IsGamepadConnected(HGamepad gamepad);

    /**
     * Check if a touch device is connected.
     *
     * @name IsTouchDeviceConnected
     * @param context context in which to search
     * @return If a touch device is connected or not
     */
    bool IsTouchDeviceConnected(HTouchDevice device);

    /**
     * Check if an accelerometer is connected.
     *
     * @name IsAccelerometerConnected
     * @param context context in which to search
     * @return If an accelerometer is connected or not
     */
    bool IsAccelerometerConnected(HContext context);

    /**
     * Obtain a keyboard packet reflecting the current input state of a HID context.
     * @note If no keyboard is connected, the internal buffers will not be changed, out_packet will not be updated,
     * and the function returns false.
     *
     * @param context context from which to retrieve the packet
     * @param out_packet Keyboard packet out argument
     * @return If the packet was successfully updated or not.
     */
    bool GetKeyboardPacket(HKeyboard keyboard, KeyboardPacket* out_packet);

    /**
     * Get text-input package
     * @note The function clears the internal buffer and subsequent calls will return an empty package (size 0)
     * If no keyboard is connected, the internal buffers will not be changed, out_packet will not be updated,
     * and the function returns false.
     * @param context context
     * @param out_packet package
     * @return If the packet was successfully updated or not.
     */
    bool GetTextPacket(HContext context, TextPacket* out_packet);

    /**
     * Get marked text-input package
     * @note The function clears the internal buffer and subsequent calls will return an empty package (size 0)
     * If no keyboard is connected, the internal buffers will not be changed, out_packet will not be updated,
     * and the function returns false.
     * @param context context
     * @param out_packet package
     * @return If the packet was successfully updated or not.
     */
    bool GetMarkedTextPacket(HContext context, MarkedTextPacket* out_packet);

    /**
     * Obtain a gamepad packet reflecting the current input state of the gamepad in a  HID context.
     *
     * @param gamepad gamepad handle
     * @param out_packet Gamepad packet out argument
     * @return If the packet was successfully updated or not.
     */
    bool GetGamepadPacket(HGamepad gamepad, GamepadPacket* out_packet);

    /**
     * Obtain a touch device packet reflecting the current input state of a HID context.
     *
     * @param context context from which to retrieve the packet
     * @param out_packet Touch device packet out argument
     * @return If the packet was successfully updated or not.
     */
    bool GetTouchDevicePacket(HTouchDevice device, TouchDevicePacket* out_packet);

    bool GetAccelerationPacket(HContext context, AccelerationPacket* out_packet);

    /**
     * Convenience function to retrieve the state of a key from a keyboard packet.
     *
     * @param packet Keyboard packet
     * @param key The requested key
     * @return If the key was pressed or not
     */
    bool GetKey(KeyboardPacket* packet, Key key);

    /**
     * Add a keyboard character input
     *
     * @param context context handle
     * @param chr The character (unicode)
     */
    void AddKeyboardChar(HContext context, int chr);

    /**
     * Set current marked text
     *
     * @param context context handle
     * @param text The marked text string
     */
    void SetMarkedText(HContext context, char* text);

    /**
     * Set the connectivity status (usually only when changed) for a gamepad index.
     *
     * @param context context handle
     * @param gamepad index of gamepad
     * @param connected connectivity status, true for connected, false for disconnected
     */
    void SetGamepadConnectivity(HContext context, int gamepad, bool connected);

    /**
     * Show keyboard if applicable
     * @param context context
     * @param type keyboard type
     * @param autoclose close keyboard automatically when outside
     */
    void ShowKeyboard(HContext context, KeyboardType type, bool autoclose);

    /**
     * Hide keyboard
     * @param context context
     */
    void HideKeyboard(HContext context);

    /**
     * Reset keyboard
     * @param context context
     */
    void ResetKeyboard(HContext context);

    /**
     * Convenience function to retrieve the state of a gamepad button from a gamepad packet.
     * @param packet Gamepad packet
     * @param button The requested button
     * @return If the button was pressed or not
     */
    bool GetGamepadButton(GamepadPacket* packet, uint32_t button);

    /**
     * Convenience function to retrieve the state of a gamepad hat from a gamepad packet.
     * @param packet Gamepad packet
     * @param hat The requested hat index
     * @param hat_value Reference to where the hat value should be written
     * @return If the hat has data or not
     */
    bool GetGamepadHat(GamepadPacket* packet, uint32_t hat, uint8_t& hat_value);

    /**
     * Convenience function to retrieve the position of a specific touch.
     * Used in unit tests (test_input.cpp)
     *
     * @param touch_index which touch to get the position from
     * @param x x-coordinate as out-parameter
     * @param y y-coordinate as out-parameter
     * @param id identifier of touch as out-parameter
     * @param pressed boolean indicating if touch is pressed as out-parameter
     * @param released boolean indicating if touch is released as out-parameter
     * @return if the position could be retrieved
     */
    bool GetTouch(TouchDevicePacket* packet, uint32_t touch_index, int32_t* x, int32_t* y, uint32_t* id, bool* pressed = 0x0, bool* released = 0x0);

    /**
     * Clears all touches.
     * Used in unit tests (test_input.cpp)
     *
     * @param context context handle
     */
    void ClearTouches(HTouchDevice device);

    /**
     * Get the name of a keyboard key.
     * @param key Keyboard key
     * @return The name of the key
     */
    const char* GetKeyName(Key key);

    /**
     * Get the name of a mouse button.
     * @param button Mouse button
     * @return The name of the button
     */
    const char* GetMouseButtonName(MouseButton button);

    /**
     * Enables the accelerometer (if available)
     */
    void EnableAccelerometer();
}

#endif // DM_HID_H
