# Phase 2 — 도메인 모델

## 목표
`architecture.md` 4절 기반의 도메인 엔티티 구현.

## 세부 작업

### 2-1. Sample 클래스
- 속성: `sampleId`, `name`, `avgProdTime`, `yield`, `stock`
- 생성자, getter, `toString()` 구현

### 2-2. Order 클래스 + OrderStatus
- `OrderStatus` enum class: `Reserved | Rejected | Producing | Confirmed | Release`
- 속성: `orderId`, `sampleId`, `customerName`, `quantity`, `status`, `createdAt`
- 주문번호 자동 발번 형식: `ORD-YYYYMMDD-XXXX`

### 2-3. ProductionJob 클래스
- 속성: `jobId`, `orderId`, `sampleId`, `shortage`, `actualQty`, `totalTime`, `startTime`
- `startTime`: 생산 시작 시각 (실시간 생산 계산 기준)

### 2-4. UserRole enum class
- `OrderManager | ProductionManager`

## 완료 기준
- 모든 모델 객체 생성 / 복사 / 출력 가능
- 빌드 경고 0건
