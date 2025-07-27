// Fill out your copyright notice in the Description page of Project Settings.


#include "PerlinProcTerrain.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
APerlinProcTerrain::APerlinProcTerrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a new Procedural Mesh Component named "Procedural Mesh"
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Procedural Mesh");

	// Set the Procedural Mesh Component as the root component of the actor
	RootComponent = ProcMesh;


}

// Called when the game starts or when spawned
void APerlinProcTerrain::BeginPlay()
{
	Super::BeginPlay();

	// Generate the vertices for the mesh
	CreateVertices();

	// Generate the triangles (indices) for the mesh
	CreateTriangles();

	// Create a new mesh section on the procedural mesh using the generated vertices, triangles, normals, UVs, vertex colors, and empty tangents
	// The last 'true' enables collision for this mesh section
	ProcMesh->CreateMeshSection(sectionID, Vertices, Triangles, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>(), true);

	// Apply the material 'Mat' to the first mesh section (index 0)
	ProcMesh->SetMaterial(0, Mat);

	
}

// Called every frame
void APerlinProcTerrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APerlinProcTerrain::AlterMesh(FVector impactPoint)
{
	// Loop through all vertices in the mesh
	for (int i = 0; i < Vertices.Num(); i++)
	{
		// Calculate vector from the actor's location to the impact point
		FVector tempVector = impactPoint - this->GetActorLocation();

		// Check if the current vertex is within the specified radius of the impact point
		if (FVector(Vertices[i] - tempVector).Size() < radius)
		{
			// Move the vertex down by 'Depth' units (altering the mesh shape)
			Vertices[i] = Vertices[i] - Depth;

			// Update the procedural mesh section with the modified vertices to reflect the change visually
			ProcMesh->UpdateMeshSection(sectionID, Vertices, Normals, UV0, UpVertexColors, TArray<FProcMeshTangent>());
		}
	}
}


void APerlinProcTerrain::CreateVertices()
{
	// Loop through grid positions in X dimension
	for (int X = 0; X <= XSize; X++)
	{
		// Loop through grid positions in Y dimension
		for (int Y = 0; Y <= YSize; Y++)
		{
			// Calculate Z height using Perlin noise for smooth terrain variation
			float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;

			// Debug message to display the calculated Z value on screen
			GEngine->AddOnScreenDebugMessage(-1, 999.0f, FColor::Yellow, FString::Printf(TEXT("Z %f"), Z));

			// Add the vertex with X, Y scaled positions and computed Z height
			Vertices.Add(FVector(X * Scale, Y * Scale, Z));

			// Add the UV coordinate corresponding to the vertex for texture mapping
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}


void APerlinProcTerrain::CreateTriangles()
{
	int Vertex = 0; // Starting index for vertices to build triangles

	// Loop through the grid in X dimension 
	for (int X = 0; X < XSize; X++)
	{
		// Loop through the grid in Y dimension
		for (int Y = 0; Y < YSize; Y++)
		{
			// Define the two triangles (v0,v1,v2 and v1,v3,v2) that make up each quad on the grid

			Triangles.Add(Vertex);              // v0 - top-left vertex of quad
			Triangles.Add(Vertex + 1);          // v1 - top-right vertex of quad
			Triangles.Add(Vertex + YSize + 1);  // v2 - bottom-left vertex of quad

			Triangles.Add(Vertex + 1);          // v1 - top-right vertex of quad
			Triangles.Add(Vertex + YSize + 2);  // v3 - bottom-right vertex of quad
			Triangles.Add(Vertex + YSize + 1);  // v2 - bottom-left vertex of quad

			// Move to the next vertex in the row
			Vertex++;
		}

		// Skip the last vertex of the row to start next row properly
		Vertex++;
	}
}


