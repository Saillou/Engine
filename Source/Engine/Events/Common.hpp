#pragma once

// To avoid glfw include
enum KeyCode {
	Space      = 32,
	Escape	   = 256,

	ArrowRight = 262,
	ArrowLeft  = 263,
    ArrowDown  = 264,
    ArrowUp    = 265,

	Numpad_0   = 320,

	ShiftLeft  = 340,
	ShiftRight = 344,
};

enum MouseButton {
	Left   = 0,
	Right  = 1,
	Middle = 2,
};

enum InputAction {
	Released = 0,
	Pressed,
	Repeated
};