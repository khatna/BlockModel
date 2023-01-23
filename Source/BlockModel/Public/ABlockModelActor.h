// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "ABlockModelActor.generated.h"

/**
 * 
 */
UCLASS()
class BLOCKMODEL_API ABlockModelActor : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:
	ABlockModelActor();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FVector> Centroids;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FVector> Dimensions;

	UFUNCTION()
	void AddBlock(FVector Centroid, FVector Dimension);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	URuntimeMeshProviderStatic* StaticProvider;

public:
	void GenerateMesh();
};
