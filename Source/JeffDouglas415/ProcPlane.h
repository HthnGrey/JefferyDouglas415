// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProcPlane.generated.h"

class UProceduralMeshComponent;

UCLASS()
class JEFFDOUGLAS415_API AProcPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProcPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostActorCreated() override;
	
	virtual void PostLoad() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
		TArray<FVector> Vertices; //an array to hold verices data

	UPROPERTY(EditAnywhere)
		TArray<int> Triangles; //an array to hold triangles in ints

	UPROPERTY(EditAnywhere)
		TArray<FVector2D> UV0; //array to hold UVs

	UPROPERTY(EditAnywhere)
		UMaterialInterface* PlaneMat;

	UFUNCTION()
	void CreateMesh(); //function for createmesh

private:
	UProceduralMeshComponent* procMesh;

};
