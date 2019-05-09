wrapped_phase/

SUMMARY
	CalcPhi: phi = (conj(I0).* I2);
	GetAngle: p = atan2(imag(phi), real(phi));


USAGE:

        // Get Phi
        typedef itk::CalcPhi<ImageTypeDoubleVecVol >  FilterTypePhi;
        FilterTypePhi::Pointer filterPhi = FilterTypePhi::New();
        filterPhi->SetInputI0(I0);
        filterPhi->SetInputI2(I2);
        filterPhi->Update();
        typename ImageTypeDoubleVecVol::Pointer Phi = filterPhi->GetOutput();

        // Get wrapped phase
        typedef itk::GetAngle<ImageTypeDoubleVecVol, ImageTypeDoubleVol >  FilterTypeAngle;
        FilterTypeAngle::Pointer filterAngle = FilterTypeAngle::New();
        filterAngle->SetInput(Phi);
        filterAngle->Update();
        typename ImageTypeDoubleVol::Pointer wrappedPhase = filterAngle->GetOutput();
~                                                                                                                 
~                                                                                               
