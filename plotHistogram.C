{
    
    enum { kMaxData = 10^5 };
    
    // be careful with bin edges and number of bins -- a good start is to have
    // one bin per integer.  This can be made more coarse, but when doing this
    // be sure the each bin has the same number of integer values.
    
    int nBins = 5;
    float lowEdge = -0.5;
    float highEdge = 4.5;
    
    TH1F hist( "hist", "Poisson Data", nBins, lowEdge, highEdge );
    
    hist.GetXaxis()->SetTitle( "Number of Counts per Run" );
    hist.GetYaxis()->SetTitle( "Number of Runs" );
    
    ifstream in;
    in.open( "run_1.txt" );
    
    int numData = 0;
    float mean = 0;
    int lineCounter = 0;
    
    bool inPreamble = true;
    
    while( !in.eof() && numData < kMaxData ){
        
        int runIndex, value;
        string line;
        
        lineCounter++;
        
        if( inPreamble ){
            
            getline( in, line );
            cout << line;
            cout << endl;
            
            // seven lines of Vernier stuff at the start of every file
            if( lineCounter == 7 ) inPreamble = false;
        }
        else{
            
            in >> runIndex >> value;
            
            mean += value;
            numData++;
            
            hist.Fill( value );
        }
    }
    
    // draw the spectrum with error bars
    hist.Draw( "E" );
    
    // divide by the number of points to get the average
    mean /= numData;
    cout << "The average value is:  " << mean << endl;
    
    // now make a second historgram that contains the theoretical prediction
    // this is a little complicated since the Poisson distribution is a discrete
    // probability distribution -- it is only valid for integral values
    
    TH1F histPred( "histPred", "Poisson Prediction", nBins, lowEdge, highEdge );
    
    // go one below and one above the edges just to be safe -- contents will end
    // up in the overflow bins if they don't land in the limits of the histogram
    int lowInt = (int)lowEdge - 1;
    int highInt = (int)highEdge + 1;
    
    for( int n = lowInt; n <= highInt; ++n ){
        
        float predictedEvents = TMath::Poisson( n, mean ) * numData;
        
        // the second argument is the weight -- by default it is set to 1, but we
        // want to increment the bins by an amount equal to the predicted number of
        // events for some value of n
        
        histPred.Fill( n, predictedEvents );
    }
    histPred.SetLineWidth( 2 );
    histPred.SetLineColor( kRed );
    
    // draw this over the top of the existing histogram with the data
    
    histPred.Draw( "same" );
    
    // we can plot a Gaussian function also
    
    gROOT->ProcessLine( ".L fitFuncs.C" );
    
    // the second argument is the function loaded in the file above
    TF1 gaussian( "gaussian", gauss, -1E6, 1E6, 3 );
    gaussian.SetParameters( (highEdge-lowEdge)/nBins*numData, mean, sqrt( mean ) );
    gaussian.SetLineColor( kBlue );
    gaussian.Draw( "same" );
    
    // the function above will appear as a user function in the Fit Panel when we choose to
    // try to fit the data -- we can also create a similar Poisson function
    
    TF1 poisson( "poisson", poiss, 0, 1E6, 2 );
    poisson.SetParameters( (highEdge-lowEdge)/nBins*numData, mean );
    poisson.SetLineColor( kGreen );
    poisson.Draw( "same" );
    
}


