// Fill out your copyright notice in the Description page of Project Settings.


#include "ProcMeshFromStatic.h"
#include "KismetProceduralMeshLibrary.h"

// Sets default values
AProcMeshFromStatic::AProcMeshFromStatic()
{
	// Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create a Procedural Mesh Component and assign it to procMesh
	procMesh = CreateDefaultSubobject<UProceduralMeshComponent>("Proc Mesh");

	// Create a Static Mesh Component and assign it to baseMesh
	baseMesh = CreateDefaultSubobject<UStaticMeshComponent>("Base Mesh");

	// Set the procedural mesh as the root component of the actor
	RootComponent = procMesh;

	// Attach the static mesh to the procedural mesh
	baseMesh->SetupAttachment(procMesh);
}


// Called when the game starts or when spawned
void AProcMeshFromStatic::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProcMeshFromStatic::PostActorCreated()
{
	Super::PostActorCreated();
	GetMeshData();
}

void AProcMeshFromStatic::PostLoad()
{
	Super::PostLoad();
	GetMeshData();
}

void AProcMeshFromStatic::GetMeshData()
{
	// Get the static mesh currently assigned to the baseMesh component
	UStaticMesh* mesh = baseMesh->GetStaticMesh();

	if (mesh)
	{
		// Extract mesh section data (vertices, triangles, normals, UVs, tangents)
		UKismetProceduralMeshLibrary::GetSectionFromStaticMesh(mesh, 0, 0, Vertices, Triangles, Normals, UV0, Tangents);

		// Update the procedural mesh section with the extracted data
		procMesh->UpdateMeshSection(0, Vertices, Normals, UV0, UpVertexColors, Tangents);

		// Call CreateMesh to perform any additional mesh operations
		CreateMesh();
	}
}


void AProcMeshFromStatic::CreateMesh()
{
	if (baseMesh)
	{
		// Create a new procedural mesh section using the provided mesh data
		procMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, UpVertexColors, Tangents, true);
	}
}


// Called every frame
void AProcMeshFromStatic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
