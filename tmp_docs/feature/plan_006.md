# Phase 6 — Service 계층 : 생산라인 / 실시간 생산

## 목표
`prd.md` 3-6, 3-8 생산라인 및 실시간 생산 처리 구현.

## 세부 작업

### 6-1. ProductionService — 생산 큐 관리
- `enqueue()`: 생산 작업 큐에 추가 (FIFO), `startTime` 기록
- `getQueue()`: 대기 중인 생산 작업 목록 반환
- `getCurrentJob()`: 현재 처리 중인 작업 반환

### 6-2. ProductionService — 실시간 생산량 계산
- `getElapsedQty()`: 조회 시각 기준 경과 생산량 계산
  - `floor((현재 시각 - startTime) / avgProdTime)`
- `checkCompletion()`: 경과 생산량 ≥ 실 생산량 시 생산 완료 처리
  - 실 생산량 전량 재고 반영
  - 주문 상태 `Producing` → `Confirmed` 전환
  - 다음 생산 작업 자동 시작

### 6-3. 생산량 계산 공식 적용
- 실 생산량: `ceil(부족분 / (수율 × 0.9))`
- 총 생산시간: `avgProdTime × actualQty`

## 완료 기준
- 생산 큐 등록 및 FIFO 순서 처리 확인
- 경과 시간에 따른 생산량 계산 확인
- 생산 완료 시 재고 반영 및 상태 전환 확인
