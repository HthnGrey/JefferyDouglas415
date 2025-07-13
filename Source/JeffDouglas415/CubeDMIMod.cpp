// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeDMIMod.h"
#include "JeffDouglas415Character.h"
#include "Kismet/KismetMathLibrary.h" //for the random function


// Sets default values
ACubeDMIMod::ACubeDMIMod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//create the subobjects
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Component");
	cubeMesh = CreateDefaultSubobject<UStaticMeshComponent>("Cube Mesh");


	//setup attachements
	RootComponent = boxComp;
	cubeMesh->SetupAttachment(boxComp);

}

// Called when the game starts or when spawned
void ACubeDMIMod::BeginPlay()
{
	Super::BeginPlay();
	//call overlap
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ACubeDMIMod::OnOverlapBegin);

	if (baseMat) //if basemat is valid
	{
		dmiMat = UMaterialInstanceDynamic::Create(baseMat, this); //create dynamic material instance and set it to the dmimat
	}
	if (cubeMesh)//if cubemesh is valid
	{
		cubeMesh->SetMaterial(0, dmiMat);//set the material of the cubemesh to dmimat
	}
	
}

// Called every frame
void ACubeDMIMod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACubeDMIMod::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& sweepResult)
{
	AJeffDouglas415Character* overlappedActor = Cast<AJeffDouglas415Character>(OtherActor);//check if overlapped actor is player

	if (overlappedActor) //if overlapped actor is valid
	{
		//Get random number in range for each ranNum
		float ranNumX = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumY = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);
		float ranNumZ = UKismetMathLibrary::RandomFloatInRange(0.f, 1.f);

		FVector4 randColor = FVector4(ranNumX, ranNumY, ranNumZ, 1.f);
		if (dmiMat) //If dmiMat is valid
		{
			dmiMat->SetVectorParameterValue("Color", randColor);
			dmiMat->SetScalarParameterValue("Darkness", ranNumX);
		}
	}

}

