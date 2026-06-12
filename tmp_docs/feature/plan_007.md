# Phase 7 — Controller / View 계층

## 목표

`docs/feature/prd_001.md ~ prd_007.md` UI 스펙 기반으로 전체 콘솔 UI 구현.  
현재 `src/main.cpp`의 임시 UI를 MVC 구조로 교체.

---

## 세부 작업

### 7-1. 공통 유틸리티

#### ConsoleHelper (`src/util/ConsoleHelper.h/.cpp`)
- `printHeader(text)` — Cyan 색상, 구분선 포함
- `printDivider()` — `──────` 구분선
- `printDoubleDivider()` — `══════` 이중 구분선
- `printBadge(text, color)` — 상태 배지 `[TEXT]` 색상 출력
- `printProgressBar(percent, width=10)` — `████░░` 형식
- `printSuccess(text)` — Green
- `printError(text)` — Red
- `printWarning(text)` — Yellow
- `getCurrentTimeString()` — `YYYY-MM-DD HH:MM:SS` 반환 (`std::chrono`)
- `getCurrentDateString()` — `YYYYMMDD` 반환

#### InputHelper (`src/util/InputHelper.h/.cpp`)
- `promptWString(label)` — Yellow 프롬프트 + wstring 입력
- `promptInt(label)` — Yellow 프롬프트 + 정수 입력 (실패 시 재입력)
- `promptDouble(label)` — Yellow 프롬프트 + 실수 입력
- `promptYN(label)` — `[Y]/[N]` 선택 입력

#### OrderIdGenerator (`src/util/OrderIdGenerator.h/.cpp`)
- `generate(allOrders)` — `ORD-YYYYMMDD-XXXX` 형식 발번
- XXXX: 전체 주문 수 + 1 기준 4자리 패딩

### 7-2. MainView / MainController

#### MainView (`src/view/MainView.h/.cpp`)
- `showHeader(currentTime)` — 시스템 현황 + 시각 출력
- `showSummary(sampleCount, totalStock, orderCount, queueCount)` — 요약 정보
- `showMenu()` — 2열 메뉴 레이아웃

#### MainController (`src/controller/MainController.h/.cpp`)
- 생성자: `MainController(SampleService&, OrderService&, ProductionService&)`
- `run()` — 메인 루프
- 요약 정보 계산 + MainView 호출
- 서브 Controller 라우팅

### 7-3. SampleView / SampleController

#### SampleView (`src/view/SampleView.h/.cpp`)
- `showMenu()` — 서브메뉴
- `showSampleList(samples, page)` — 5건 페이지네이션 테이블
- `showSampleDetail(sample)` — 등록 결과
- `showSearchResult(samples)` — 검색 결과 테이블
- `showRegisterForm()` — 등록 입력 프롬프트
- `showRegisterSuccess(sampleId)` — 등록 성공 메시지

#### SampleController (`src/controller/SampleController.h/.cpp`)
- `run(SampleService&)`
- 등록 / 목록 / 검색 흐름 처리
- 페이지 이동: `[N]` 다음 / `[P]` 이전

### 7-4. OrderView / OrderController

#### OrderView (`src/view/OrderView.h/.cpp`)
- `showMenu()` — 시료 주문 진입
- `showOrderForm()` — 입력 프롬프트
- `showOrderConfirm(sample, customerName, quantity)` — 입력 내용 확인
- `showOrderSuccess(order)` — 주문번호 + 상태 배지

#### OrderController (`src/controller/OrderController.h/.cpp`)
- `run(OrderService&, SampleService&)`
- 입력 → 확인 → [Y/N] → 등록 흐름

### 7-5. ApprovalView / ApprovalController

#### ApprovalView (`src/view/ApprovalView.h/.cpp`)
- `showReservedList(orders)` — 번호 선택 테이블
- `showStockCheck(sample, order)` — 재고 확인 + 부족분 표시
- `showApprovalConfirm(isShortage, shortage, actualQty, totalTime)` — 승인 확인 메시지
- `showApprovalResult(order)` — 상태 배지 전환 결과

#### ApprovalController (`src/controller/ApprovalController.h/.cpp`)
- `run(OrderService&, SampleService&, ProductionService&)`
- 번호 선택 → 재고 확인 → [Y/N] → 승인/거절 흐름

### 7-6. MonitoringView / MonitoringController

#### MonitoringView (`src/view/MonitoringView.h/.cpp`)
- `showMenu(currentTime)` — 서브메뉴 + 조회 시각
- `showOrderStats(reserved, confirmed, producing, release)` — 상태별 배지 + 건수
- `showStockStatus(samples)` — 시료별 재고 + 진행바 + 상태 배지

#### MonitoringController (`src/controller/MonitoringController.h/.cpp`)
- `run(SampleService&, OrderService&)`

### 7-7. ProductionView / ProductionController

#### ProductionView (`src/view/ProductionView.h/.cpp`)
- `showHeader()` — FIFO 방식 제목
- `showCurrentJob(job, sample, elapsedQty, currentTime)` — 박스 + 진행바
- `showQueue(jobs, samples)` — 대기 큐 테이블 + 예상 완료 시각

#### ProductionController (`src/controller/ProductionController.h/.cpp`)
- `run(ProductionService&, SampleService&, OrderService&)`
- 조회 시각 기준 `checkCompletion` 호출 후 현황 표시

### 7-8. ReleaseView / ReleaseController

#### ReleaseView (`src/view/ReleaseView.h/.cpp`)
- `showConfirmedList(orders, samples)` — 번호 선택 테이블
- `showReleaseResult(order, releasedAt)` — 처리일시 + 상태 배지

#### ReleaseController (`src/controller/ReleaseController.h/.cpp`)
- `run(OrderService&, SampleService&)`

### 7-9. main.cpp 교체

- 임시 UI 제거, MVC 구조로 완전 교체
- `MainController` 생성 + `run()` 호출

---

## 완료 기준

- 전체 메뉴 흐름 정상 동작
- 색상 규칙 (prd.md 4절) 일치
- 페이지네이션 (5건/페이지)
- 주문번호 `ORD-YYYYMMDD-XXXX` 발번
- 진행바 `████░░` 형식 정상 출력
- Debug|x64 테스트 전체 통과
- Release|x64 빌드 경고 0건
