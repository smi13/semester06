FUNC(FunctionPhi0, (x[0] - 2) * (x[0] - 2) * (x[0] - 2) * (x[0] - 2) + 
                   (x[0] - 2 * x[1]) * (x[0] - 2 * x[1]));

FUNC(FunctionPhi1, x[0] * x[0] - x[1]);

VECTOR_FUNC(GradPhi0, res[0] = 2 * (x[0] - 2 * x[1]) +
                               4 * (x[0] - 2) * (x[0] - 2) * (x[0] - 2); 
                      res[1] = -4 * (x[0] - 2 * x[1]););

VECTOR_FUNC(GradPhi1, res[0] = 2 * x[0]; res[1] = -1.0);

FunctionPhi0 phi0;
GradPhi0 phi0_grad;

FunctionPhi1 phi1;
GradPhi1 phi1_grad;

void PrepareTask( MdTask &T )
{
   T.setTaskDimension(2);
   T.setMainFunc(phi0);
   T.setMainFuncGrad(phi0_grad);

   T.addFunc(phi1);

   T.addGradFunc(phi1_grad);
}
