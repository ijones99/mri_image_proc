SUMMARY:
	ShiftPhaseRange: Matlab loads the phase data with a range of [0, 4096];; itk loads with a range of [-4096, 4096]. This filter makes a linear conversion to the Matlab format [0, 4096]: (data+4096*)/2

*or arbitrary value

USAGE:

	
   	typedef itk::ShiftPhaseRange< TImage> ShiftFilterType;
    	typename ShiftFilterType::Pointer shiftFilter1 = ShiftFilterType::New();
    	shiftFilter1->SetInput(reader[ phaseInd[y[0]]]->GetOutput());
    	shiftFilter1->SetShiftFactor(4096);
    	typename TImage::Pointer phase1Shifted = shiftFilter1->GetOutput();
	
INPUT: 
	3d volume

OUTPUT: 

	3d volume

