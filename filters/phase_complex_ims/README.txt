phase_complex_ims/

SUMMARY:
	MagToComplex: convert magnitude and phase volumes to a complex volume (I0,I1,I2)
	PhaseToRad: Convert the phase volume to radians (S(i).phase/max_phase*2*pi - pi; )
	SplitComplex: Split a complex volume into the real and imaginary parts (in two separate volumes).
	
USAGE:

        // MagToComplex
        typedef itk::MagToComplex<ImageTypeDoubleVol,ImageTypeDoubleVol, ImageTypeDoubleVecVol >  FilterTypeComplex;
        FilterTypeComplex::Pointer filterComplex0 = FilterTypeComplex::New();
        filterComplex0->SetInputImage1(imageS1mag);
        filterComplex0->SetInputImage2(imageS1PhaseRad);
        filterComplex0->Update();
        typename ImageTypeDoubleVecVol::Pointer I0 = filterComplex0->GetOutput();
	
	// PhaseToRad
        typedef itk::PhaseToRad<ImageTypeDoubleVol>  FilterTypePhase2Rad;
        FilterTypePhase2Rad::Pointer filterPhaseToRad1 = FilterTypePhase2Rad::New();
        filterPhaseToRad1->SetInput(imageS1phase );
        filterPhaseToRad1->SetMaxPhaseVal(MaxPhaseVal );
        filterPhaseToRad1->Update();
        typename ImageTypeDoubleVol::Pointer imageS1PhaseRad = filterPhaseToRad1->GetOutput();
~                                                                                                              

        // split data I0
        typedef itk::SplitComplex<ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVol >  FilterSplitComplex;
        FilterSplitComplex::Pointer filterSplitComplex = FilterSplitComplex::New();
        filterSplitComplex->SetInput(I0);
        filterSplitComplex->Update();
        typename ImageTypeDoubleVol::Pointer I0_real = filterSplitComplex->GetOutput1();
        typename ImageTypeDoubleVol::Pointer I0_imag = filterSplitComplex->GetOutput2();


	
