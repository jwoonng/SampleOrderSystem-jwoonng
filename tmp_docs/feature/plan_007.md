# Phase 7 — Controller / View 계층

## 목표
`prd.md` 3-1, 3-5 콘솔 UI 및 입력 처리 구현.

## 세부 작업

### 7-1. 공통 유틸리티
- `InputValidator`: 숫자 입력, 범위 검증, 빈 문자열 검증
- `ConsoleHelper`: 화면 구분선, 헤더 출력, 조회 시각 표시

### 7-2. MainController / MainView
- 시작 시 `UserRole` 선택 (주문 담당자 / 생산 담당자)
- 역할별 메뉴 표시 및 라우팅
- 메인 화면 요약 정보 표시 (등록 시료 수, 총 재고, 전체 주문 수, 생산라인 대기 수)

### 7-3. 도메인별 Controller / View
- `SampleController` / `SampleView`: 시료 등록 / 조회 / 검색
- `OrderController` / `OrderView`: 주문 접수, 승인/거절, 출고 처리
- `ProductionController` / `ProductionView`: 생산라인 현황 및 대기 큐
- `MonitoringController` / `MonitoringView`: 상태별 주문 수, 시료별 재고 상태

### 7-4. 모니터링 화면
- 조회 시각 표시
- 재고 상태 표기: 여유 / 부족 / 고갈
- `REJECTED` 주문 제외

## 완료 기준
- 전체 메뉴 흐름 수동 테스트 통과
- 역할별 메뉴 접근 제어 확인
- 잘못된 입력 시 재입력 유도 확인
