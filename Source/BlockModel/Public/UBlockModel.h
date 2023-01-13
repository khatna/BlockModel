// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ABlockModelActor.h"
#include "UBlockModel.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FBlockProperties
{
	GENERATED_BODY()

	// map from property column name to property value
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, float> PropertyMap;
};

UCLASS(Blueprintable,BlueprintType)
class BLOCKMODEL_API UBlockModel : public UObject
{
	GENERATED_BODY()
	
public:
	// map from block ID to actor
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<ABlockModelActor*> Blocks;

	// Min and max of properties
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FString SelectedProperty = "";
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<FString, float> PropertyMin;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<FString, float> PropertyMax;
	
	// map from block ID to property list
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FBlockProperties> BlockProperties;

	// default material instance (can be edited in runtime based on properties)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInstance* BlockMaterial;
	
private:
	// spawns blocks
	UFUNCTION(BlueprintCallable)
	void ImportBlockModel(FString Path, FVector UTMRef);
};
