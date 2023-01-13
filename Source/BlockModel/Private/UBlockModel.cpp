// Fill out your copyright notice in the Description page of Project Settings.


#include "UBlockModel.h"
#include "Kismet/KismetStringLibrary.h"

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
	
	// Populate property values
	for (int i = 1; i < Rows.Num(); i++)
	{
		TArray<FString> Values = UKismetStringLibrary::ParseIntoArray(Rows[i], ",");
		
		ABlockModelActor* BlockActor = GetWorld()->SpawnActor<ABlockModelActor>(
			FVector(0.0, 0.0,0.0),
			FRotator(0.0,0.0,0.0)
		);
		
		FBlockProperties Properties;
		for (int j = 0; j < Columns.Num(); j++)
		{
			FString ColName = Columns[j];
			float Value = FCString::Atof(*Values[j]);
			
			// populate actor dimensions and centroid
			if (ColName.Equals("x"))
			{
				BlockActor->Centroid.Y = (Value - UTMRef.X) * 100.0f;
				continue;	
			}
			if (ColName.Equals("y"))
			{
				BlockActor->Centroid.X = (Value - UTMRef.Y) * 100.0f;
				continue;
			}
			if (ColName.Equals("z"))
			{
				BlockActor->Centroid.Z = (Value - UTMRef.Z) * 100.0f;
				continue;
			}
			if (ColName.Equals("dx"))
			{
				BlockActor->Dimensions.Y = Value * 100.0f;
				continue;
			}
			if (ColName.Equals("dy"))
			{
				BlockActor->Dimensions.X = Value * 100.0f;
				continue;
			}
			if (ColName.Equals("dz"))
			{
				BlockActor->Dimensions.Z = Value * 100.0f;
				continue;
			}

			Properties.PropertyMap.Add(ColName, Value);

			// add to property min and max map (for visualization)
			if (i == 1)
			{
				PropertyMin.Add(ColName, Value);
				PropertyMax.Add(ColName, Value);
			}

			// update property min and max map (for visualization)
			if (PropertyMin.Find(ColName) && *PropertyMin.Find(ColName) > Value)
				PropertyMin.Add(ColName, Value);
			if (PropertyMax.Find(ColName) && *PropertyMax.Find(ColName) < Value)
				PropertyMax.Add(ColName, Value);
		}

		// add actor and properties to maps
		Blocks.Add(BlockActor);
		BlockProperties.Add(Properties);

		// Generate mesh for block actor
		BlockActor->Material = BlockMaterial;
		BlockActor->GenerateMesh();
	}
}
