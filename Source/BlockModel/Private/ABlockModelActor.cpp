// Fill out your copyright notice in the Description page of Project Settings.


#include "ABlockModelActor.h"
#include "Providers/RuntimeMeshProviderStatic.h"

// Sets default values
ABlockModelActor::ABlockModelActor()
{
	StaticProvider = CreateDefaultSubobject<URuntimeMeshProviderStatic>(TEXT("Static Provider"));
	StaticProvider->SetShouldSerializeMeshData(false);
	PrimaryActorTick.bCanEverTick = false;
}

void ABlockModelActor::AddBlock(FVector Centroid, FVector Dimension)
{
	Centroids.Add(Centroid);
	Dimensions.Add(Dimension);
}

void ABlockModelActor::GenerateMesh()
{
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->ClearSection(0, 0);
	
	TArray<FVector> Positions;
	TArray<FVector> Normals;
	TArray<int32> Triangles;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> TexCoords;

	FVector AllCorners[3][4] = {
		{ FVector(0.5, 0.5, 0.5), FVector(0.5, 0.5, -0.5), FVector(0.5, -0.5, 0.5), FVector(0.5, -0.5, -0.5) },
		{ FVector(0.5, 0.5, 0.5), FVector(0.5, 0.5, -0.5), FVector(-0.5, 0.5, 0.5), FVector(-0.5, 0.5, -0.5) },
		{ FVector(0.5, 0.5, 0.5), FVector(0.5, -0.5, 0.5), FVector(-0.5, 0.5, 0.5), FVector(-0.5, -0.5, 0.5) },
	};

	TArray<FVector> AllNormals = {
		FVector(1.0f, 0.0f, 0.0f),
		FVector(0.0f, 1.0f, 0.0f),
		FVector(0.0f, 0.0f, 1.0f)
	};
	
	for (int i = 0; i < Centroids.Num(); i++)
	{
		// 3 axes, 2 orientations = 6 sides
		for (int axs = 0; axs < 3; axs++)
			for (int sign = -1; sign <= 1; sign += 2)
			{
				for (FVector Corner : AllCorners[axs])
				{
					FVector Pos = sign * Corner * Dimensions[i] + Centroids[i];
					Positions.Add(Pos);
					Normals.Add(sign * AllNormals[axs]);
				}

				int Offset = Positions.Num();
				if (sign == 1) Triangles.Append({ Offset, Offset+3, Offset+2, Offset, Offset+1, Offset+3 });
				else           Triangles.Append({ Offset, Offset+2, Offset+3, Offset, Offset+3, Offset+1 });
			}
	}
	
	// Create RMC section
	const TArray<FColor> EmptyColors{};
	StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
	StaticProvider->SetupMaterialSlot(0, TEXT("BIM Material"), Material);
}
