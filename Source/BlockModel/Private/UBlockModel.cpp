// Fill out your copyright notice in the Description page of Project Settings.


#include "UBlockModel.h"
#include "Kismet/KismetStringLibrary.h"

void UBlockModel::HideBlockModel()
{
	for (const TPair<FString,ABlockModelActor*>& Pair : BlockGroups)
	{
		Pair.Value->SetActorHiddenInGame(true);	
	}
}

// TODO: validation (see if required columns exist)
void UBlockModel::ImportBlockModel(FString Path, FVector UTMRef)
{
	UE_LOG(LogTemp, Warning, TEXT("Import Started"));
	TArray<FString> Rows;
	FFileHelper::LoadFileToStringArray(Rows, *Path);
	TArray<FString> Columns = UKismetStringLibrary::ParseIntoArray(Rows[0], ",");

	// make a dummy property that has the same value for all blocks, in the case
	// where no property is provided by the csv file
	Headers.Add("header");

	// make each column name lowercase for consistency
	// add the name (header) of the property for future interactivity
	for (int i = 0; i < Columns.Num(); i++)
	{
		Columns[i] = Columns[i].ToLower();
		if (Columns[i] != "x" && Columns[i] != "y" && Columns[i] != "z" &&
			Columns[i] != "dx" && Columns[i] != "dy" && Columns[i] != "dz")
		{
			Headers.Add(Columns[i]);
		}
	}
	
	// Read and store all block information
	for (int i = 1; i < Rows.Num(); i++)
	{
		TArray<FString> Values = UKismetStringLibrary::ParseIntoArray(Rows[i], ",");

		FBlock Block;
		Block.Centroid = FVector(0.0f, 0.0f, 0.0f);
		Block.Dimension = FVector(0.0f, 0.0f, 0.0f);

		for (int j = 0; j < Columns.Num(); j++)
		{
			
			FString ColName = Columns[j];
			FString Value = Values[j];
			
			// populate actor dimensions and centroid
			if (ColName.Equals("x"))
			{
				Block.Centroid.Y = (FCString::Atod(*Value) - UTMRef.X);
				continue;	
			}
			if (ColName.Equals("y"))
			{
				Block.Centroid.X = (FCString::Atod(*Value) - UTMRef.Y);
				continue;
			}
			if (ColName.Equals("z"))
			{
				Block.Centroid.Z = (FCString::Atod(*Value) - UTMRef.Z);
				continue;
			}
			if (ColName.Equals("dx"))
			{
				Block.Dimension.Y = FCString::Atod(*Value);
				continue;
			}
			if (ColName.Equals("dy"))
			{
				Block.Dimension.X = FCString::Atod(*Value);
				continue;
			}
			if (ColName.Equals("dz"))
			{
				Block.Dimension.Z = FCString::Atod(*Value);
				continue;
			}

			Block.HeaderVals.Add(ColName,Value);
		}

		// add dummy header
		Block.HeaderVals.Add("header","value");
		
		Blocks.Add(Block);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Import End"));
}

void UBlockModel::SelectProperty(FString Property)
{
	UE_LOG(LogTemp, Warning, TEXT("Property Select"))
	
	if (!Headers.Contains(Property))
		return;

	// reset block groups map, implicitly despawning blocks due to GC
	HideBlockModel();
	BlockGroups.Reset();

	for (FBlock Block : Blocks)
	{
		if (!Block.HeaderVals.Find(Property)) continue;

		FString Val = *Block.HeaderVals.Find(Property);
		ABlockModelActor* BlockActor;

		// spawn block group actor if first block in group
		if (!BlockGroups.Find(Val))
		{
			BlockActor = GetWorld()->SpawnActor<ABlockModelActor>(
				FVector(0.0, 0.0,0.0),
				FRotator(0.0,0.0,0.0)
			);
			BlockActor->Material = BlockMaterial;
			BlockGroups.Add(Val, BlockActor);
		}

		BlockActor = *BlockGroups.Find(Val);
		BlockActor->AddBlock(Block.Centroid, Block.Dimension);
	}

	UE_LOG(LogTemp, Warning, TEXT("Mesh Generate"))
	for (const TPair<FString,ABlockModelActor*>& Pair : BlockGroups)
	{
		Pair.Value->GenerateMesh();	
	}

	
	UE_LOG(LogTemp, Warning, TEXT("Change materials"))
	PostprocessBlocks();
	UE_LOG(LogTemp, Warning, TEXT("Property Select end"))
}
