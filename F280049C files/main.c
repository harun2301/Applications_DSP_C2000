#include "f28x_project.h"   // Ya incluye Devices y Examples

#define N   1000

extern int filtroIIR(int ,int *,int *);
extern void goertzel(int*, float*);

extern int xn[], yn[];          // el tamanio se define en filtroIIR()
extern float spXk[], spYk[];    // el tamanio se define en goertzel()

void main(void){

    //InitGpio();
    //GPIO_SetupPinOptions(13,0,0);   // GPIO13, input, pull-down;


    int coef_b[3] = {4096,-8155,4096};
    // a0 = 4096, a0 = -7992, a0 = 3933,
    // pero hay que pasarlos negados
    int coef_a[3] = {-4096,7992,-3933};
    int i;

    goertzel(&xn[0], &spXk[0]);

    for(i=0; i<N; i++){
        // x[n] pasa en AL
        // &coef_b[0] pasa en XAR4
        // &coef_a[0] pasa en XAR5
        yn[i] = filtroIIR(xn[i], &coef_b[0], &coef_a[0]);
    }

    goertzel(&yn[0], &spYk[0]);

    while(1);
}
