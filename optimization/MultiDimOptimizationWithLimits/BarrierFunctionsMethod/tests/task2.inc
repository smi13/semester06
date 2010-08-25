FUNC(FunctionPhi0, x[0] * x[0] + x[1] * x[1] + 2 * x[2] * x[2] + 
	           x[3] * x[3] - 5 * x[0] - 5 * x[1] - 21 * x[2] + 7 * x[3]);

FUNC(FunctionPhi1, -8 + x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + 
                   x[3] * x[3] + x[0] - x[1] + x[2] - x[3]);
FUNC(FunctionPhi2, -10 + x[0] * x[0] + 2 * x[1] * x[1] + x[2] * x[2] + 
                   2 * x[3] * x[3] - x[0] + x[3]);
FUNC(FunctionPhi3, -5 + 2 * x[0] * x[0] + x[1] * x[1] + x[2] * x[2] +
                   2 * x[0] - x[1] - x[3]);

VECTOR_FUNC(GradPhi0, res[0] = 2 * x[0] - 5; 
                      res[1] = 2 * x[1] - 5;
                      res[2] = 4 * x[2] - 21;
                      res[3] = 2 * x[3] + 7;);

VECTOR_FUNC(GradPhi1, res[0] = 2 * x[0] + 1; 
		      res[1] = 2 * x[1] - 1; 
 	              res[2] = 2 * x[2] + 1; 
		      res[3] = 2 * x[3] - 1;);

VECTOR_FUNC(GradPhi2, res[0] = 2 * x[0] - 1; 
		      res[1] = 4 * x[1]; 
		      res[2] = 2 * x[2]; 
		      res[3] = 4 * x[3] + 1;);

VECTOR_FUNC(GradPhi3, res[0] = 4 * x[0] + 2; 
		      res[1] = 2 * x[1] - 1; 
		      res[2] = 2 * x[2]; 
		      res[3] = -1;);

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
   T.setTaskDimension(4);
   T.setMainFunc(phi0);
   T.setMainFuncGrad(phi0_grad);

   T.addFunc(phi1);
   T.addFunc(phi2);
   T.addFunc(phi3);

   T.addGradFunc(phi1_grad);
   T.addGradFunc(phi2_grad);
   T.addGradFunc(phi3_grad);
}
