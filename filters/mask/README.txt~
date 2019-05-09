SUMMARY:
	CreateMask: create a 3D mask from I0
	ApplyMask: Apply mask to a volume
	NormMask: Normalize mask to range of [0,1]
	ScaleImage: scale an image by a given factor	

USAGE:

	//CREATE MASK
        typedef itk::CreateMask<ImageTypeDoubleVecVol, ImageTypeDoubleVol >  FilterTypeMask;
        FilterTypeMask::Pointer filterMask = FilterTypeMask::New();
        filterMask->SetInput(I0);
        filterMask->SetMaskThreshold(MaskThreshold);
        filterMask->Update();
        typename ImageTypeDoubleVol::Pointer mask = filterMask->GetOutput();

        //ApplyMask
        typedef itk::ApplyMask<ImageTypeDoubleVol, ImageTypeDoubleVol, ImageTypeDoubleVol> ApplyMaskWc;
        ApplyMaskWc::Pointer wcMaskFilt = ApplyMaskWc::New();
        wcMaskFilt ->SetInput(wc1);
        wcMaskFilt ->SetInputMask(maskNorm);
        wcMaskFilt->Update();
        typename ImageTypeDoubleVol::Pointer wcMask = wcMaskFilt->GetOutput();

        //NormMask
        typedef itk::NormMask<ImageTypeDoubleVol >  FilterTypeNormMask;
        FilterTypeNormMask::Pointer filterNormMask = FilterTypeNormMask::New();
        filterNormMask->SetInputMask(mask);
        filterNormMask->Update();
        typename ImageTypeDoubleVol::Pointer maskNorm = filterNormMask->GetOutput();

        //Scale Image
        typedef itk::ScaleImage< ImageTypeDoubleVol> ScaleFilterType;
        ScaleFilterType::Pointer scaleFilter = ScaleFilterType::New();
        scaleFilter->SetInput(mask);
        scaleFilter->SetScaleFactor(MaskScaleFactor);
        typename ImageTypeDoubleVol::Pointer maskScale = scaleFilter->GetOutput();


	
INPUT: 
	3d volume, I0 volume for CreateMask.

OUTPUT: 

	3d volume

