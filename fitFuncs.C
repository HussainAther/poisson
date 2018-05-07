
double gauss( double* x, double* par ){

  double expArg = 0;

  if( par[2] ){
    
    expArg = ( x[0] - par[1] ) / par[2];
  }
   
  return par[0] / ( par[2] * sqrt( 2*TMath::Pi() ) ) * TMath::Exp(-0.5*expArg*expArg);
}

double poiss( double* x, double* par ){

  // this function uses the continuous version of a Poisson distribution

  return par[0] * TMath::Poisson( x[0], par[1] );
}
