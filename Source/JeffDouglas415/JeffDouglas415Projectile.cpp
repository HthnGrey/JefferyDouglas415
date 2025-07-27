// Copyright Epic Games, Inc. All Rights Reserved.

#include "JeffDouglas415Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PerlinProcTerrain.h"

AJeffDouglas415Projectile::AJeffDouglas415Projectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
    

    // Bind the OnHit function to the CollisionComp's OnComponentHit event, so OnHit is called when a collision occurs
    CollisionComp->OnComponentHit.AddDynamic(this, &AJeffDouglas415Projectile::OnHit);


	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	//create ballmesh object
	ballMesh = CreateDefaultSubobject<UStaticMeshComponent>("Ball Mesh");

	// Set as root component
	RootComponent = CollisionComp;
	
	//set up attachment for ballmesh to collision
	ballMesh->SetupAttachment(CollisionComp);

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AJeffDouglas415Projectile::BeginPlay()
{
	Super::BeginPlay();
	//get random color
	randColor = FLinearColor(UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.0f), UKismetMathLibrary::RandomFloatInRange(0.f, 1.f), 1.f);
	//create dynamic material instance
	dmiMat = UMaterialInstanceDynamic::Create(projMat, this);
	//set material of ball mesh to dmimat
	ballMesh->SetMaterial(0, dmiMat);
	//set the color
	dmiMat->SetVectorParameterValue("ProjColor", randColor);

}

void AJeffDouglas415Projectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // Check if the other actor and component are valid, and if physics simulation is enabled on the other component
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
    {
        // Apply an impulse at the projectile's location to the hit component, scaled by velocity
        OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

        // Destroy this projectile after applying impulse
        Destroy();
    }

    // If the other actor is valid (regardless of physics)
    if (OtherActor != nullptr)
    {
        // If the color particle system is set
        if (colorP)
        {
            // Spawn a Niagara particle system attached to the hit component with a slight offset
            UNiagaraComponent* particleComp = UNiagaraFunctionLibrary::SpawnSystemAttached(colorP, HitComp, NAME_None, FVector(-20.f, 0.f, 0.f), FRotator(0.f), EAttachLocation::KeepRelativeOffset, true);

            // Set the particle system’s "RandColor" parameter to a random color
            particleComp->SetNiagaraVariableLinearColor(FString("RandColor"), randColor);

            // Destroy the projectile's mesh visually
            ballMesh->DestroyComponent();

            // Disable collision for the projectile's collision component
            CollisionComp->BodyInstance.SetCollisionProfileName("NoCollision");
        }

        // Pick a random frame number between 0 and 3 
        float frameNum = UKismetMathLibrary::RandomFloatInRange(0.f, 3.f);

        // Spawn a decal at the hit location with random size and orientation based on hit normal
        auto Decal = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), baseMat, FVector(UKismetMathLibrary::RandomFloatInRange(20.F, 40.F)), Hit.Location, Hit.Normal.Rotation(), 0.f);

        // Create a dynamic material instance for the decal
        auto MatInstance = Decal->CreateDynamicMaterialInstance();

        // Set decal color parameter to the random color
        MatInstance->SetVectorParameterValue("Color", randColor);

        // Set decal frame parameter to the random frame
        MatInstance->SetScalarParameterValue("Frame", frameNum);

        // Attempt to cast the hit actor to a PerlinProcTerrain to modify terrain mesh
        APerlinProcTerrain* procTerrain = Cast<APerlinProcTerrain>(OtherActor);

        // If cast succeeds, call the mesh alteration function at the impact point
        if (procTerrain)
        {
            procTerrain->AlterMesh(Hit.ImpactPoint);
        }
    }
}



