# STM32 Driver Design Policy

## Purpose

This project is an educational bare-metal STM32 driver library.

Its primary goals are to:

- understand how STM32 peripherals work at the register level;
- practice good embedded-driver and API design;
- keep the relationship between C code and hardware visible.

Clarity and educational value are preferred over reproducing every feature or abstraction provided by commercial HAL libraries.

---

## 1. Defaults

- Do not silently depend on important MCU reset-state defaults.
- Build peripheral configuration registers from known values where practical.
- When a zero-valued register field intentionally represents a configuration, make that behavior clear in the code or comments.
- Defaults that affect application behavior must be documented.

Example:

```c
uint16_t cr1_register = 0;

case SPI_MODE_FULL_DUPLEX:
    /* BIDIMODE = 0, RXONLY = 0 */
    break;
```

---

## 2. Configuration Validation

- Public APIs should reject obviously invalid arguments.
- Unsupported configuration values must not silently fall through.
- Prefer named constants or typed enums over arbitrary numeric values.
- Detect invalid configuration combinations where reasonably practical.
- Validation should remain simple enough that it does not hide the underlying hardware behavior.

---

## 3. Runtime State Changes

Initialization establishes the peripheral's **starting state**, not necessarily its permanent state.

Hardware state that legitimately changes during operation should be controlled through dedicated driver functions.

For example:

```c
SPI_set_half_duplex_direction(SPI1, SPI_DIRECTION_TX);

SPI_send_data(...);

SPI_set_half_duplex_direction(SPI1, SPI_DIRECTION_RX);

SPI_receive_data(...);
```

The application should normally express its intent, such as `TX` or `RX`, while the driver performs the corresponding register manipulation.

Direct register access remains available for educational experimentation.

---

## 4. Error Handling

- Do not silently ignore invalid configuration or arguments.
- Functions capable of meaningful failure should return a simple status value.
- Keep the error system small and understandable.

Example:

```c
typedef enum
{
    SPI_OK = 0,
    SPI_ERROR_INVALID_ARGUMENT,
    SPI_ERROR_INVALID_CONFIG,
    SPI_ERROR_TIMEOUT
} SPI_Status_t;
```

Compile-time errors such as `#error` should be reserved for conditions that make the build fundamentally invalid, such as compiling MCU-specific code for an unsupported target.

---

## 5. Documentation

Public APIs should document, where relevant:

- what the function does;
- valid arguments;
- important assumptions;
- starting/default behavior;
- hardware state changed by the function;
- runtime side effects;
- possible errors.

Hardware-specific code should use the relevant register and bit names.

Comments should explain **why** important register operations are performed rather than merely repeating what the C statement does.

Example:

```c
/* BIDIOE (Bidirectional Output Enable):
 * 1 = transmit
 * 0 = receive
 */
```

---

## Design Principle

The library should provide enough abstraction to demonstrate proper driver design without hiding the STM32 hardware being studied.

When choosing between hidden behavior, excessive abstraction, and explicit hardware behavior, prefer:

**explicit hardware behavior wrapped in a small, understandable API.**

---

# Code-Review Checklist

Before accepting new driver code, check:

- **Hardware behavior:** Does the implementation match the STM32 reference manual and use the correct registers, bits, and fields?
- **Magic numbers:** Are hardware constants represented by meaningful definitions unless a literal is intentionally clearer and documented?
- **Defaults:** Are important starting states explicit rather than silently dependent on MCU reset values?
- **Validation:** Are invalid arguments, unsupported values, and invalid configuration combinations detected where practical?
- **Runtime state:** If hardware state legitimately changes after initialization, is there an appropriate driver operation for changing it?
- **Errors:** Are meaningful failures reported rather than silently ignored?
- **Abstraction:** Does the public API express user intent while keeping register manipulation inside the driver?
- **Educational value:** Can the relationship between the API operation and the underlying STM32 hardware still be understood?
- **Comments:** Do comments explain important hardware reasoning rather than obvious C syntax?
- **Documentation:** Are new assumptions, defaults, side effects, and errors documented?
- **Simplicity:** Does each abstraction or feature solve a real design problem or teach an important hardware concept rather than merely copying a commercial HAL?

## Final Review Question

> Can someone reading this code understand both **what the STM32 hardware is doing** and **why the driver was designed this way**?

If not, clarify the implementation before accepting it.
