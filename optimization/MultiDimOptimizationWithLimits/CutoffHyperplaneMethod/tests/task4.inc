FUNC(FunctionPhi0, x[0] * x[0] * x[0] * x[0] + x[1] * x[1] * x[1] * x[1] + sqrt(2 + x[0] * x[0] + x[1] * x[1]));

FUNC(FunctionPhi1, x[0] * x[0] + x[1] - 0.3);
FUNC(FunctionPhi2, x[0] + x[1] - 5);
FUNC(FunctionPhi3, x[0] * x[0] + x[1] * x[1] - 10);

VECTOR_FUNC(GradPhi0, res[0] = 4 * x[0] * x[0] * x[0] + x[0] / sqrt(2 + x[0] * x[0] + x[1] * x[1]); 
                      res[1] = 4 * x[1] * x[1] * x[1] + x[1] / sqrt(2 + x[0] * x[0] + x[1] * x[1]););

VECTOR_FUNC(GradPhi1, res[0] = 2 * x[0]; res[1] = -1.0;);
VECTOR_FUNC(GradPhi2, res[0] = 1; res[1] = 1;);
VECTOR_FUNC(GradPhi3, res[0] = 2 * x[0]; res[1] = 2 * x[1];);

FunctionPhi0 phi0;
GradPhi0 phi0_grad;

FunctionPhi1 phi1;
FunctionPhi2 phi2;
FunctionPhi3 phi3;

GradPhi1 phi1_grad;
GradPhi2 phi2_grad;
GradPhi3 phi3_grad;

void PrepareTask( MdTask &T )
{
   T.setTaskDimension(2);
   T.setMainFunc(phi0);
   T.setMainFuncGrad(phi0_grad);

   T.addFunc(phi1);
   T.addFunc(phi2);
   T.addFunc(phi3);

   T.addGradFunc(phi1_grad);
   T.addGradFunc(phi2_grad);
   T.addGradFunc(phi3_grad);
}
