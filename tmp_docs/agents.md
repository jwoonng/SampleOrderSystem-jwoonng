# agents.md — 에이전트 정의

각 에이전트의 역할과 담당 업무를 정의하는 파일.

---

## 에이전트 구성

```
┌─────────────────────────────────────┐
│         Orchestration Agent         │
│  - 전체 흐름 총괄 및 단계 전환 결정  │
└──────────────┬──────────────────────┘
               │ 지시 / 결과 수신
    ┌──────────┼──────────────┐
    ▼          ▼              ▼
┌────────┐ ┌────────┐ ┌──────────────┐
│  Red   │ │ Green  │ │  Verifying   │
│ Agent  │ │ Agent  │ │    Agent     │
└────────┘ └────────┘ └──────────────┘
```

---

## 에이전트별 역할

### Orchestration Agent

- Red / Green / Verifying Agent를 총괄한다.
- 각 에이전트의 수행 결과를 검토하고, 다음 단계로 넘어갈지 Red를 재수행할지 결정한다.
- 현재 Phase가 정상 완료됐다고 판단하면 개발자에게 수행 사항을 공유하고 컨펌을 요청한다.

### Red Agent

- 요구사항(`docs/prd.md`)에 맞춰 TODO 리스트 형태로 Unit Test 코드를 작성한다.
- 테스트는 아직 구현이 없으므로 반드시 실패 상태여야 한다.
- Green Agent가 구현할 범위를 명확히 정의하는 것이 목적이다.

### Green Agent

- Red Agent가 작성한 테스트를 모두 통과하는 최소 구현 코드를 작성한다.
- 테스트 통과 후 Clean Code / SRP 원칙에 맞게 리팩터링한다.
- 리팩터링 후에도 모든 테스트가 통과하는 상태를 유지한다.

### Verifying Agent

- Green Agent가 구현한 기능이 요구사항을 충족하는지 검증한다.
- Regression Test: 기존에 동작하던 기능이 깨지지 않았는지 확인한다.
- Safety Test: 경계값, 예외 입력, 재고 부족 등 엣지 케이스를 검증한다.

---

## 실행 순서

세부 Phase별로 아래 순서를 반복한다.

```
Red → Green → Verifying → Orchestration 판단
                                │
              ┌─────────────────┴──────────────────┐
           통과                                  실패
              │                                    │
        다음 Phase                  Green / Verifying → Orchestration
     (Phase 완료 시                  부족 사항 보고 수신
    개발자 컨펌 요청)                       │
                                    Orchestration → Red
                                    부족 사항 정리 후 재수행 요청
```

- Green과 Verifying은 결과를 **Orchestration에게만 보고**한다.
- Red에 직접 전달하지 않으며, Orchestration이 부족 사항을 정리하여 Red에게 재수행을 요청한다.

### 단계 전환 규칙

| 상황 | 보고 대상 | Orchestration 결정 |
|------|----------|--------------------|
| Verifying 통과 | Orchestration | 다음 Phase로 전환 |
| Green 실패 (테스트 미통과) | Orchestration | 부족 사항 정리 후 Red 재수행 요청 |
| Verifying 실패 (요구사항 불충족 / Regression 발생) | Orchestration | 부족 사항 정리 후 Red 재수행 요청 |
| 현재 Phase 정상 완료 | 개발자 | 수행 사항 공유 및 컨펌 요청 |
