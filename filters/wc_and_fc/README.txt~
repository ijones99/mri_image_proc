wc_and_fc/

SUMMARY
	FilterFatContent: Apply a median filter (2d filter operation)
	GetFc: Get fat content: fc = 0.5*(abs(I0) - pc.*abs(I1)./A);      
	GetPc: compute PC: pc = cos(angle(I1_prime.*exp(-Im*phi)));       
	GetWc: Get water content: wc = 0.5*(abs(I0) + pc.*abs(I1)./A);

USAGE:

        //Get wc: wc = fc./(fc+wc)*100;
        typedef itk::FilterFatContent<ImageTypeDoubleVol> FilterTypeFiltFatContent;
        FilterTypeFiltFatContent::Pointer fatContentFilter = FilterTypeFiltFatContent::New();
        fatContentFilter->SetInputFr(fr);
        fatContentFilter->SetInputWc(wcMask);
        fatContentFilter->SetInputFc(fcMask);
        fatContentFilter->Update();
        typename ImageTypeDoubleVol::Pointer fcFilt = fatContentFilter->GetOutputFc();
        typename ImageTypeDoubleVol::Pointer wcFilt = fatContentFilter->GetOutputWc();

        // compute fat contents
        typedef itk::GetFc<ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVol >  GetFc;
        GetFc::Pointer fcFilter1 = GetFc::New();
        fcFilter1 ->SetInputI0(I0);
        fcFilter1 ->SetInputPc(Pc);
        fcFilter1 ->SetInputI1(I1);
        fcFilter1 ->SetInputA(A);
        fcFilter1 ->SetFcScale(FC_SCALE);
        fcFilter1 ->SetSMax(FC_S_MAX);
        fcFilter1->Update();
        typename ImageTypeDoubleVol::Pointer fc1 = fcFilter1->GetOutput();

        // compute pc
        typedef itk::GetPc<ImageTypeDoubleVecVol, ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVol >  GetPc;
        GetPc::Pointer pcFilter = GetPc::New();
        pcFilter->SetInputI1(I1);
        pcFilter->SetInputI0(I0);
        pcFilter->SetInputPhi(unwrappedPhase); // this is for some reason referred to as "phi" in Matlab
        pcFilter->Update();
        typename ImageTypeDoubleVol::Pointer Pc = pcFilter->GetOutput();


        // compute water contents
        typedef itk::GetWc<ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVecVol, ImageTypeDoubleVol, ImageTypeDoubleVol >  GetWc;
        GetWc::Pointer wcFilter1 = GetWc::New();
        wcFilter1 ->SetInputI0(I0);
        wcFilter1 ->SetInputPc(Pc);
        wcFilter1 ->SetInputI1(I1);
        wcFilter1 ->SetInputA(A);
        wcFilter1 ->SetWcScale(WC_SCALE);
        wcFilter1 ->SetSMax(WC_S_MAX);
        wcFilter1->Update();
        typename ImageTypeDoubleVol::Pointer wc1 = wcFilter1->GetOutput();

