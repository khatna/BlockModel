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

void ABlockModelActor::GenerateMesh()
{
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
	
	// 3 axes, 2 orientations = 6 sides
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	for (int axs = 0; axs < 3; axs++)
		for (int sign = -1; sign <= 1; sign += 2)
		{
			const int Section = axs * 2 + (sign + 1) / 2;
			StaticProvider->ClearSection(0, Section);
			
			TArray<FVector> Positions;
			TArray<FVector> Normals;
			TArray<int32> Triangles;
			TArray<FRuntimeMeshTangent> Tangents;
			TArray<FVector2D> TexCoords;

			for (FVector Corner : AllCorners[axs])
			{
				FVector Pos = sign * Corner * Dimensions + Centroid;
				Positions.Add(Pos);
				Normals.Add(sign * AllNormals[axs]);
			}

			if (sign == 1) Triangles.Append({ 0, 3, 2, 0, 1, 3 });
			else           Triangles.Append({ 0, 2, 3, 0, 3, 1 });
			
			// Create RMC section
			const TArray<FColor> EmptyColors{};
			StaticProvider->CreateSectionFromComponents(0, Section, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
			StaticProvider->SetupMaterialSlot(0, TEXT("BIM Material"), Material);
		}
}