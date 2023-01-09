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

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector Centroid;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FVector Dimensions;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
	URuntimeMeshProviderStatic* StaticProvider;

public:
	void GenerateMesh();
};
