# Architecture — 반도체 시료 생산주문관리 시스템

**작성일**: 2026-06-12

---

## 1. 레이어 구조

```
┌────────────────────────────────────────┐
│               View Layer               │  콘솔 UI 출력 / 입력 수신
│   MainView, SampleView, OrderView ...  │
└───────────────────┬────────────────────┘
                    │
┌───────────────────▼────────────────────┐
│            Controller Layer            │  사용자 입력 처리 / 흐름 제어
│  MainController, SampleController ...  │
└───────────────────┬────────────────────┘
                    │
┌───────────────────▼────────────────────┐
│             Service Layer              │  비즈니스 로직
│  SampleService, OrderService ...       │
└───────────────────┬────────────────────┘
                    │
┌───────────────────▼────────────────────┐
│           Repository Layer             │  데이터 접근 추상화
│  ISampleRepository, IOrderRepository  │
│  JsonSampleRepository ...             │
└───────────────────┬────────────────────┘
                    │
┌───────────────────▼────────────────────┐
│              Model Layer               │  도메인 엔티티
│  Sample, Order, ProductionJob          │
└────────────────────────────────────────┘
```

---

## 2. 디렉터리 구조

```
SampleOrderSystem/
├── src/
│   ├── model/          # 도메인 엔티티 (Sample, Order, ProductionJob)
│   ├── view/           # 콘솔 UI (메뉴 출력, 입력 프롬프트)
│   ├── controller/     # 입력 처리 및 서비스 호출
│   ├── service/        # 비즈니스 로직
│   ├── repository/     # Repository 인터페이스 + JSON 구현체
│   └── util/           # 입력 검증, 콘솔 헬퍼, 날짜 유틸
├── data/               # JSON 영속성 파일 저장 경로
├── tests/              # 단위 테스트 (서비스/모델 계층)
└── docs/               # 문서
```

---

## 3. 역할 구분

로그인/인증 없이 **메뉴 진입 시 역할을 선택**하여 접근 범위를 구분한다.

```cpp
enum class UserRole
{
    OrderManager,       // 주문 담당자 — 주문 접수, 모니터링
    ProductionManager   // 생산 담당자 — 시료 등록, 주문 승인/거절, 생산라인, 출고
};
```

| 역할 | 접근 가능 메뉴 |
|------|---------------|
| `OrderManager` (주문 담당자) | 시료 주문, 모니터링 |
| `ProductionManager` (생산 담당자) | 시료 관리, 주문 승인/거절, 생산라인 조회, 출고 처리 |

- `MainController`가 현재 `UserRole`을 보유하고 메뉴 접근을 제어한다.
- 고객(주문자)은 시스템 사용자가 아니므로 `Order.customerName` 문자열로만 표현한다.

---

## 4. 핵심 도메인 모델

```
Sample
  - sampleId   : wstring   // S-001 형식
  - name       : wstring
  - avgProdTime: double     // 단위: min/ea
  - yield      : double     // 0.0 ~ 1.0
  - stock      : int

Order
  - orderId    : wstring   // ORD-YYYYMMDD-XXXX 형식
  - sampleId   : wstring
  - customerName: wstring
  - quantity   : int
  - status     : OrderStatus
  - createdAt  : wstring

OrderStatus (enum class)
  Reserved | Rejected | Producing | Confirmed | Release

ProductionJob
  - jobId      : wstring
  - orderId    : wstring
  - sampleId   : wstring
  - shortage   : int       // 부족분
  - actualQty  : int       // 실 생산량 = ceil(shortage / (yield * 0.9))
  - totalTime  : double    // avgProdTime * actualQty
```

---

## 5. 생산량 계산 공식

```
실 생산량  = ceil(부족분 / (수율 × 0.9))
총 생산시간 = 평균 생산시간 × 실 생산량
```

---

## 6. 데이터 영속성

- 저장 방식: **JSON 파일** (`data/` 디렉터리)
- 파일 구성: `samples.json`, `orders.json`, `production_queue.json`
- 저장 시점: 생성·수정·삭제 발생 시 즉시 기록
- 로드 시점: 애플리케이션 시작 시 전체 로드

---

## 7. 생산라인 동작 방식

- 단일 생산 라인, 스케줄링 전략: **FIFO 큐**
- 주문 승인 시 재고 부족이면 `ProductionJob`을 큐에 자동 추가
- 생산 완료 처리 시 재고 증가 + 주문 상태 `Producing → Confirmed`

---

## 8. 주요 의존 관계

```
Controller → Service → IRepository ← JsonRepository
                ↓
             Model (도메인 객체)
```

- Controller는 Service에만 의존
- Service는 IRepository 인터페이스에만 의존 (구현체 직접 참조 금지)
- View는 데이터를 const 참조로만 수신 (읽기 전용)
