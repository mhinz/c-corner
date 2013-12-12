#include <stdio.h>
#include <math.h>

enum shapes {
    Rectangle,
    Circle,
    Square,
};

typedef struct {
    enum shapes type;
    union {
        struct { int width, height; } rdata;
        struct { int radius;        } cdata;
        struct { int side;          } sdata;
    } data;
} shape_t;

double area(shape_t *s) {
    switch (s->type) {
        case Rectangle: return (s->data.rdata.width * s->data.rdata.height);
        case Circle:    return (3.14 * s->data.cdata.radius * s->data.cdata.radius);
        case Square:    return (s->data.sdata.side * s->data.sdata.side);
    }
    return 0;
}

int main(void) {
    printf("Rectangle: %f\n", area(&(shape_t){Rectangle, {3, 4}}));
    printf("Rectangle: %f\n", area(&(shape_t){Circle,    {5}}   ));
    printf("Rectangle: %f\n", area(&(shape_t){Square,    {5}}   ));
}
