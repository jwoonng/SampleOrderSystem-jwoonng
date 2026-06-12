# Phase 5 — Service 계층 : 주문 관리

## 목표
`prd.md` 3-3, 3-4, 3-7 주문 흐름 비즈니스 로직 구현.

## 세부 작업

### 5-1. OrderService — 주문 접수
- `placeOrder()`: 주문 생성, 상태 `Reserved`
- 등록되지 않은 시료 ID 검증
- 주문 수량 양수 검증

### 5-2. OrderService — 주문 승인/거절
- `approveOrder()`: 재고 확인 후 분기 처리
  - 재고 충분: 재고 차감 → 상태 `Confirmed`
  - 재고 부족: `ProductionService`에 생산 등록 요청 → 상태 `Producing`
- `rejectOrder()`: 상태 `Rejected`

### 5-3. OrderService — 출고 처리
- `releaseOrder()`: `Confirmed` 상태 주문 → `Release`
- `Confirmed` 상태가 아닌 주문 출고 시도 시 예외 처리

## 완료 기준
- 주문 접수 → 승인 → 출고 전체 흐름 동작 확인
- 재고 분기(충분 / 부족) 처리 확인
- 거절 처리 확인
