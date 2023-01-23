// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ABlockModelActor.h"
#include "UBlockModel.generated.h"

// Block model download callback type
DECLARE_DYNAMIC_DELEGATE(FDownloadDelegate);

USTRUCT(BlueprintType)
struct FBlock
{
	GENERATED_BODY()
	
	UPROPERTY()
	FVector Centroid;

	UPROPERTY()
	FVector Dimension;

	UPROPERTY()
	TMap<FString, FString> HeaderVals;
	
};

UCLASS(Blueprintable,BlueprintType)
class BLOCKMODEL_API UBlockModel : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadonly)
	TArray<FBlock> Blocks;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TArray<FString> Headers;

	// map from column value to RMC block group
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<FString,ABlockModelActor*> BlockGroups;
	
	// Min and max of properties
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FString SelectedProperty = "";

	// default material instance (can be edited in runtime based on properties)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UMaterialInstance* BlockMaterial;

	// hide the block model from render
	UFUNCTION(BlueprintCallable)
	void HideBlockModel();

	// called after re-selecting property. delegates postprocessing (like colors)
	// to blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void PostprocessBlocks();
private:
	// imports block model data over HTTP
	UFUNCTION(BlueprintCallable)
	void DownloadBlockModel(FString Path, FVector UTMRef, const FDownloadDelegate& Callback);
	
	// imports block model data from disk
	UFUNCTION(BlueprintCallable)
	void ImportBlockModel(FString Path, FVector UTMRef);
	
	UFUNCTION()
	void ProcessRows(TArray<FString> Rows, FVector UTMRef);
	
	// select a new property and regenerate block RMCs
	UFUNCTION(BlueprintCallable)
	void SelectProperty(FString Property);
};

