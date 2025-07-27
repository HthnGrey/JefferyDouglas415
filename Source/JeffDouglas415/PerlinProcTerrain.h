// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PerlinProcTerrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class JEFFDOUGLAS415_API APerlinProcTerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerlinProcTerrain();

// How we generate the size of our terrain; cannot go below zero
UPROPERTY(EditAnywhere, Meta = (ClampMin = 0)) 
int XSize = 0;

UPROPERTY(EditAnywhere, Meta = (ClampMin = 0)) // Same as above for Y dimension
int YSize = 0;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0)) // Multiplier for the height of the noise (terrain elevation)
float ZMultiplier = 1.0f;

UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0)) // Controls the scale of the Perlin noise input
float NoiseScale = 1.0f;

UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001)) // Scale of the terrain in world units, must be positive
float Scale = 0;

UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001)) // UV texture scale, must be positive
float UVScale = 0;

UPROPERTY(EditAnywhere) // Radius of the hole (used in mesh alteration)
float radius;

UPROPERTY(EditAnywhere) // Depth vector of the hole (how deep the alteration affects the mesh)
FVector Depth;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* Mat; // Material applied to the procedural mesh

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Function to modify the mesh based on an impact point
    UFUNCTION()
    void AlterMesh(FVector impactPoint);

private:
    UProceduralMeshComponent* ProcMesh; // Procedural mesh component to render the mesh

    // Arrays storing mesh data
    TArray<FVector> Vertices;        // Mesh vertices positions
    TArray<int> Triangles;           // Indices defining triangles
    TArray<FVector2D> UV0;           // Texture coordinates for vertices
    TArray<FVector> Normals;         // Vertex normals for lighting
    TArray<FColor> UpVertexColors;   // Vertex colors

    int sectionID = 0;               // Mesh section identifier

    // Functions to generate mesh data
    void CreateVertices();
    void CreateTriangles();

};

