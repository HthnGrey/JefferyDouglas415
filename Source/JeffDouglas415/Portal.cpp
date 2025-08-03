// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal.h"
#include "JeffDouglas415Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APortal::APortal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//initialize components
	mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Comp");
	sceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>("Capture");
	rootArrow = CreateDefaultSubobject<UArrowComponent>("Root Arrow");

	//make boxComp the root component
	RootComponent = boxComp;

	//attatch components
	mesh->SetupAttachment(boxComp);
	sceneCapture->SetupAttachment(mesh);
	rootArrow->SetupAttachment(RootComponent);
	

	mesh->SetCollisionResponseToAllChannels(ECR_Ignore);

}

// Called when the game starts or when spawned
void APortal::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APortal::OnOverlapBegin);
	mesh->SetHiddenInSceneCapture(true);
	//mesh->bCastStaticShadow(false);
	//mesh->bCastDynamicShadow(false);

	if (mat)
	{
		mesh->SetMaterial(0, mat);
	}
	
}

// Called every frame
void APortal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePortals();

}

void APortal::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AJeffDouglas415Character* playerChar = Cast<AJeffDouglas415Character>(OtherActor);

	if (playerChar) //if player is me
	{
		if (OtherPortal) //if otherportal is valid
		{
			if (!playerChar->isTeleporting) //if isteleporting is false
			{
				playerChar->isTeleporting = true; //set isteleporting to true
				FVector loc = OtherPortal->rootArrow->GetComponentLocation(); //get the location of the other portal
				playerChar->SetActorLocation(loc); //set playercharacter location to the other portal


				//setup a timer by function 
				FTimerHandle TimerHandle;
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindUFunction(this, "SetBool", playerChar);
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1, false);

			}
		}
	}
}

void APortal::SetBool(AJeffDouglas415Character* playerChar)
{
	playerChar->isTeleporting = false; //set isteleporting to false
}

void APortal::UpdatePortals()
{
	FVector Location = this->GetActorLocation() - OtherPortal->GetActorLocation(); //get player location
	FVector camLocation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentLocation(); //get player camera location
	FRotator camRotation = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetTransformComponent()->GetComponentRotation(); //get player camera rotation
	FVector CombinedLocation = camLocation + Location;

	sceneCapture->SetWorldLocationAndRotation(CombinedLocation, camRotation); //set the location and cam rotation to scene Capture
}


