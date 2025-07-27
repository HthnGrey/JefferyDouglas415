// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProcMeshFromStatic.generated.h"


UCLASS()
class JEFFDOUGLAS415_API AProcMeshFromStatic : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AProcMeshFromStatic();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    virtual void PostActorCreated() override;

    virtual void PostLoad() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Arrays to store mesh data for the procedural mesh

    UPROPERTY()
    TArray<FVector> Vertices;           // Vertex positions

    UPROPERTY()
    TArray<int> Triangles;              // Triangle indices

    UPROPERTY()
    TArray<FVector> Normals;            // Normals for lighting calculations

    UPROPERTY()
    TArray<FLinearColor> VertexColors;  // Linear color data per vertex

    UPROPERTY()
    TArray<FColor> UpVertexColors;      // Vertex colors in standard color format

    UPROPERTY()
    TArray<FProcMeshTangent> Tangents;  // Tangent vectors for normal mapping

    UPROPERTY()
    TArray<FVector2D> UV0;              // Texture UV coordinates

    // Reference to the base static mesh component
    UPROPERTY(EditAnywhere)
    UStaticMeshComponent* baseMesh;

private:
    UProceduralMeshComponent* procMesh; // Procedural mesh component

    // Functions to fetch mesh data from the base mesh and create the procedural mesh
    void GetMeshData();
    void CreateMesh();
};
