// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/ProjectileBase.h"
#include "AbilitySystem/GASAbilityTypes.h"


// Sets default values
AProjectileBase::AProjectileBase()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ProjectileMesh->SetCollisionObjectType(ECC_WorldDynamic);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	ProjectileMesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	ProjectileMesh->SetIsReplicated(true);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

void AProjectileBase::SetProjectileParams(const FProjectileParams& Params)
{
	if(IsValid(Params.ProjectileMesh))
	{
		ProjectileMesh->SetStaticMesh(Params.ProjectileMesh);
	}
	if(IsValid(ProjectileMovementComponent))
	{
		//ProjectileMovementComponent->SetUpdatedComponent(ProjectileMesh);
		ProjectileMovementComponent->InitialSpeed = Params.InitialSpeed;
		ProjectileMovementComponent->MaxSpeed = Params.InitialSpeed;
		ProjectileMovementComponent->ProjectileGravityScale = Params.GravityScale;
		ProjectileMovementComponent->bShouldBounce = Params.bShouldBounce;
		ProjectileMovementComponent->Bounciness = Params.Bounciness;
	}
}


