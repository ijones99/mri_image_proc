deltaB0.hxx

SUMMARY:
	COMPUTATION
	deltaB0 = delta_f./(GammaH_Hz);  % deltaB0 in T

USAGE:
	typedef itk::GetDeltaB0<ImageTypeDoubleVol, ImageTypeDoubleVol, ImageTypeDoubleVol >  GetDeltaB0;

        GetDeltaB0::Pointer deltaB0Filter = GetDeltaB0::New();
        deltaB0Filter->SetInputPhi(unwrappedPhase);
        deltaB0Filter->SetInputMask(maskNorm);
        deltaB0Filter->SetDeltaFMax(DELTA_F_MAX);
        deltaB0Filter->SetGammaH(GAMMAH_H);
        deltaB0Filter->SetTeVal1(teVal1);
        deltaB0Filter->SetTeVal2(teVal2);
        deltaB0Filter->SetTeVal3(teVal3);
        deltaB0Filter->Update();
        typename ImageTypeDoubleVol::Pointer deltaB0 = deltaB0Filter->GetOutput();

INPUT: 
	"unwrappedPhase.mha"

OUTPUT: 
	"deltaB0.mha" 

