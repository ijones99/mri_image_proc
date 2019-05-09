phase_complex_ims/

SUMMARY:
	GetA: A = sqrt(abs(I2)./abs(I0));
	GetT2Star: 
		deltaTE = (TE(3)-TE(1))/2;  % in ms
		T2star = abs(deltaTE./log(A));
	MedianFilt: Filters volume in a 2d manner, slice by slice along the z axis. Edges (1/2 of window size) are not filtered.

USAGE:
        // compute A
        typedef itk::GetA<ImageTypeDoubleVecVol, ImageTypeDoubleVecVol, ImageTypeDoubleVol >  GetA;
        GetA::Pointer aFilter = GetA::New();
        aFilter->SetInputI2(I2);
        aFilter->SetInputI0(I0);
        aFilter->SetTeVal1(teVal1);
        aFilter->SetTeVal2(teVal2);
        aFilter->SetTeVal3(teVal3);
        aFilter->Update();
        typename ImageTypeDoubleVol::Pointer A = aFilter->GetOutput();

        // compute T2Star (nonfiltered)
        typedef itk::GetT2Star<ImageTypeDoubleVol, ImageTypeDoubleVol >  GetT2Star;
        GetT2Star::Pointer t2starFilter = GetT2Star::New();
        t2starFilter->SetInputA(A);
        t2starFilter->SetT2StarMax(T2STAR_MAX);
        t2starFilter->SetTeVal1(teVal1);
        t2starFilter->SetTeVal2(teVal2);
        t2starFilter->SetTeVal3(teVal3);
        t2starFilter->Update();
        typename ImageTypeDoubleVol::Pointer t2StarUnfilt = t2starFilter->GetOutput();

        // compute T2Star (filtered)
        typedef itk::MedianFilt<ImageTypeDoubleVol, ImageTypeDoubleVol >  MedianFilt;
        MedianFilt::Pointer medianFilt = MedianFilt::New();
        medianFilt ->SetInput(t2StarUnfilt);
        medianFilt ->SetFilterSizeX(CELL_FILTER[0]);
        medianFilt ->SetFilterSizeY(CELL_FILTER[1]);
        medianFilt->Update();
        typename ImageTypeDoubleVol::Pointer t2Star = medianFilt->GetOutput();
~                                                                               
	
