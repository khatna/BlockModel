// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ABlockModelActor.h"
#include "UBlockModel.generated.h"

/**
 * 
 */
USTRUCT()
struct FBlockProperties
{
	GENERATED_BODY()

	// map from property column name to property value
	TMap<FString, float> PropertyMap;
};

UCLASS(Blueprintable,BlueprintType)
class BLOCKMODEL_API UBlockModel : public UObject
{
	GENERATED_BODY()
	
public:
	// map from block ID to actor
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<int,ABlockModelActor*> Blocks;

	// map from block ID to property list
	UPROPERTY()
	TMap<int,FBlockProperties> BlockProperties;

	// default material instance (can be edited in runtime based on properties)
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UMaterialInstance* BlockMaterial;

	UFUNCTION(BlueprintCallable)
	void SetBlockMaterial(UMaterialInstance* Material);
	
private:
	// spawns blocks
	UFUNCTION(BlueprintCallable)
	void ImportBlockModel(FString Path, FVector UTMRef);
};
