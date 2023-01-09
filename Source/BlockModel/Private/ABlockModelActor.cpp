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
	// each corner of a block
	int NumVerts = 8;
	
	// Clear RMC
	GetRuntimeMeshComponent()->Initialize(StaticProvider);
	StaticProvider->ClearSection(0, 0);

	// reserve buffers
	TArray<FVector> Positions;
	TArray<FVector> Normals;
	TArray<FRuntimeMeshTangent> Tangents;
	TArray<FVector2D> TexCoords;
	TArray<int32> Triangles;

	Normals.AddUninitialized(NumVerts);
	Tangents.AddUninitialized(NumVerts);
	TexCoords.AddUninitialized(NumVerts);

	// add vertices of block
	for (int i = -1; i <= 1; i+=2)
		for (int j = -1; j <= 1; j+=2)
			for (int k = -1; k <= 1; k+=2)
			{
				FVector Pos = Centroid + FVector(
					i * Dimensions.X / 2,
					j * Dimensions.Y / 2,
					k * Dimensions.Z / 2
				);
				Positions.Add(Pos);
			}
	
	// set triangles
	Triangles.Append({
		0, 1, 3,
		0, 3, 2,
		1, 5, 7,
		1, 7, 3,
		2, 3, 7,
		2, 7, 6,
		4, 6, 7,
		4, 7, 5,
		5, 0, 4,
		5, 1, 0,
		6, 4, 0,
		6, 0, 2
	});

	// Create RMC section
	const TArray<FColor> EmptyColors{};
	StaticProvider->CreateSectionFromComponents(0, 0, 0, Positions, Triangles, Normals, TexCoords, EmptyColors, Tangents, ERuntimeMeshUpdateFrequency::Infrequent, false);
	StaticProvider->SetupMaterialSlot(0, TEXT("BIM Material"), Material);
}