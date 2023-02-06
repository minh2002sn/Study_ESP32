#ifndef BUTTON_H

typedef void (*BUTTON_CALLBACK_FUNCTION_t) (int pin);
void BUTTON_Init(int pin);
void BUTTON_Set_Callback_Function(void *callback_function, void *ReleasingCallback);


#endif