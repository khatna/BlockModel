// Fill out your copyright notice in the Description page of Project Settings.


#include "UBlockModel.h"
#include "Kismet/KismetStringLibrary.h"

void UBlockModel::SetBlockMaterial(UMaterialInstance* Material)
{
	BlockMaterial = Material;
	// update material for each actor?
}

// TODO: validation (see if required columns exist)
void UBlockModel::ImportBlockModel(FString Path, FVector UTMRef)
{
	TArray<FString> Rows;
	FFileHelper::LoadFileToStringArray(Rows, *Path);
	TArray<FString> Columns = UKismetStringLibrary::ParseIntoArray(Rows[0], ",");

	// make each column name lowercase for consistency
	for (int i = 0; i < Columns.Num(); i++)
	{
		Columns[i] = Columns[i].ToLower();
	}
	
	// Find which column id is
	const unsigned int IdIndex = Columns.Find("id");
	if (IdIndex == INDEX_NONE) return;

	// Populate property values
	for (int i = 1; i < Rows.Num(); i++)
	{
		TArray<FString> Values = UKismetStringLibrary::ParseIntoArray(Rows[i], ",");
		int Id = FCString::Atoi(*Values[IdIndex]);
		
		ABlockModelActor* BlockActor = GetWorld()->SpawnActor<ABlockModelActor>(
			FVector(0.0, 0.0,0.0),
			FRotator(0.0,0.0,0.0)
		);
		
		FBlockProperties Properties;
		for (int j = 0; j < Columns.Num(); j++)
		{
			if (j == IdIndex) continue;
			
			FString ColName = Columns[j];
			float Value = FCString::Atof(*Values[j]);
			
			// populate actor dimensions and centroid
			if (ColName.Equals("x"))
			{
				BlockActor->Centroid.Y = Value - UTMRef.X;
				continue;	
			}
			if (ColName.Equals("y"))
			{
				BlockActor->Centroid.X = Value - UTMRef.Y;
				continue;
			}
			if (ColName.Equals("z"))
			{
				BlockActor->Centroid.Z = Value - UTMRef.Z;
				continue;
			}
			if (ColName.Equals("dx"))
			{
				BlockActor->Dimensions.Y = Value;
				continue;
			}
			if (ColName.Equals("dy"))
			{
				BlockActor->Dimensions.X = Value;
				continue;
			}
			if (ColName.Equals("dz"))
			{
				BlockActor->Dimensions.Z = Value;
				continue;
			}
			
			Properties.PropertyMap.Add(ColName, Value);
		}

		// add actor and properties to maps
		Blocks.Add(Id, BlockActor);
		BlockProperties.Add(Id, Properties);

		// Generate mesh for block actor
		BlockActor->Id = Id;
		BlockActor->Material = BlockMaterial;
		BlockActor->GenerateMesh();
	}
}
