// Fill out your copyright notice in the Description page of Project Settings.
#include "ChooseNextWaypoint.h"
#include "PatrolRoute.h"
#include "AIController.h"
#include "Containers/Array.h"

#include "BehaviorTree/BlackboardComponent.h"
EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) 
{
    Super::ExecuteTask(OwnerComp,NodeMemory);
    
    auto guard = OwnerComp.GetAIOwner()->GetPawn()->FindComponentByClass<UPatrolRoute>()->GetPatrolPoints();
    auto BlackboardComp = OwnerComp.GetBlackboardComponent();
    int index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
//    UE_LOG(LogTemp,Warning,TEXT("IN: %d,%d"),index,guard.Num());
    if(guard.Num()!=0)
    {
        if( index!=(guard.Num()) && index != -1 )
        {
            BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName,guard[index++]);
        }
        else
        {
            index=0;
            BlackboardComp->SetValueAsObject(Waypoint.SelectedKeyName,guard[index]);
        }

         BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName,index);
     return EBTNodeResult::Type::Succeeded;
    }
    else
    {
    //    UE_LOG(LogTemp,Error,TEXT("ChooseNextWaypoint: PatrolPoints Empty"))
         return EBTNodeResult::Type::Failed;
    }
   
}
