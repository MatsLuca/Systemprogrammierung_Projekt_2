#ifndef INPUT_H
#define INPUT_H

typedef struct
{
    int dx;      /* -1 left, +1 right, 0 none */
    int quit;    /* non-zero if user requested quit */
} input_action_t;

void input_init(void);
input_action_t input_poll(void);

#endif /* INPUT_H */
