#define N   1000

extern int filtroIIR(int ,int *,int *);

extern int xn[], yn[];

void main(void){

    int coef_b[3] = {4096,-8155,4096};
    int coef_a[3] = {4096,-7992,3933};
    int i;

    for(i=0; i<N; i++){
        // x[n] pasa en AL
        // &coef_b[0] pasa en XAR4
        // &coef_a[0] pasa en XAR5
        yn[i] = filtroIIR(xn[i], &coef_b[0], &coef_a[0]);
    }

    while(1);
}
