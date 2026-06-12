# CLAUDE.md — SampleOrderSystem 작업 지침

## 핵심 원칙

- **[중요]** 개발은 항상 `docs/prd.md`의 요구사항을 그대로 따른다. 판단이 필요한 상황에서는 반드시 검토를 요청하고 임의로 진행하지 않는다.
- 정의되지 않은 기능, 구조, 파일은 추가하지 않는다.
- 요구사항 범위 밖의 리팩터링, 추상화, 기능 확장은 하지 않는다.

---

## 개발 환경

- **언어**: C++20
- **IDE / 빌드**: Visual Studio 2022, MSBuild (`SampleOrderSystem.vcxproj`)
- **플랫폼**: Windows 10/11, x64 콘솔 애플리케이션
- **문자셋**: Unicode (`wstring`, `wcin`, `wcout`)
- 새 소스/헤더 파일 추가 시 `.vcxproj`에 직접 등록한다.

## 한글 인코딩 설정

### 소스 파일
- 모든 소스/헤더 파일은 **UTF-8 with BOM** 으로 저장한다 (Visual Studio 기본값).
- Visual Studio에서 파일 저장 시: `파일 > 다른 이름으로 저장 > 저장 버튼 옆 ▼ > 인코딩하여 저장 > UTF-8 with BOM` 선택.

### 콘솔 출력 (런타임)
`main()` 진입 직후 아래 설정을 반드시 적용한다.

```cpp
#include <io.h>
#include <fcntl.h>

// wide character 스트림 모드 설정 (wcout/wcin 한글 깨짐 방지)
_setmode(_fileno(stdout), _O_U16TEXT);
_setmode(_fileno(stdin),  _O_U16TEXT);
_setmode(_fileno(stderr), _O_U16TEXT);
```

- `wstring`, `wcout`, `wcin` 을 일관되게 사용한다.
- `_O_U16TEXT` 모드 적용 후 `cout` / `printf` 혼용 금지 (스트림 충돌).

### 프로젝트 설정
- `.vcxproj` 의 `<CharacterSet>` 은 `Unicode` 로 유지한다.
- 컴파일러 옵션에 `/utf-8` 을 추가하여 소스 파일 인코딩을 명시한다.
  ```xml
  <AdditionalOptions>/utf-8 %(AdditionalOptions)</AdditionalOptions>
  ```

---

## 개발 방식

### TDD (Test-Driven Development)

1. 실패하는 테스트 작성 → 2. 통과하는 최소 구현 → 3. 리팩터링
- 비즈니스 로직은 반드시 테스트를 먼저 작성한 후 구현한다.
- 테스트는 UI 계층을 거치지 않고 서비스/모델 계층을 직접 호출한다.

### Clean Code

- **[중요] SRP (단일 책임 원칙)**: 클래스와 함수는 변경 이유가 하나여야 한다. 역할이 둘 이상이면 분리한다.
- 함수는 한 가지 일만 한다.
- 이름은 의도를 드러낸다 (축약 금지).
- 주석은 "왜(WHY)"가 비자명한 경우에만 작성한다.
- 매직 넘버는 `constexpr` 상수로 대체한다.

### Claude Harness

Claude는 아래 조건 외의 작업을 자율적으로 진행하지 않는다.

| 허용 | 금지 |
|------|------|
| `docs/prd.md`에 명시된 기능 구현 | 요구사항에 없는 기능 추가 |
| `docs/architecture.md`에 정의된 구조 내에서 구현 | 아키텍처에 정의되지 않은 계층·클래스 임의 추가 |
| `tmp_docs/plan.md`에 정의된 Phase 작업 | 정의되지 않은 Phase 선진행 |
| `tmp_docs/agents.md`에 할당된 에이전트 작업 | 역할 범위 밖 작업 |
| 검토 요청 후 승인된 판단 | 임의 판단으로 구조 변경 |

---

## 아키텍처 참조 (PoC 4종)

아래 4개 프로젝트를 참조한다. **그대로 복사하지 않으며**, 이 프로젝트의 요구사항에 맞게 적용한다.

### ConsoleMVC-jwoonng — MVC 계층 분리
- `Model` / `View` / `Controller` 엄격한 계층 분리
- `IModel`, `IView`, `IController` 인터페이스 기반 설계
- `Application`이 MVC 3요소를 소유하고 메인 루프 관리
- View는 Model을 `const&`로만 참조 (읽기 전용)

### DummyDataGenerator-jwoonng — 메타데이터 기반 설계
- 열거형(`enum class`)으로 타입 안전성 확보
- 생성 로직(DataGenerator)과 저장 로직(DatabaseManager) 분리
- 콜백(`std::function`)으로 진행 상황 전달

### DataMonitor-jwoonng — 콘솔 렌더링 및 상태 관리
- Windows Console API (`HANDLE`, `GotoXY`, `SetColor`) 활용 패턴
- 렌더링 상태(`RenderState`)를 별도 구조체로 분리
- 필터/정렬 모드를 `enum class`로 정의

### DataPersistence-jwoonng — Repository 패턴
- `IRepository` 순수 가상 인터페이스 + 구현체 분리
- `std::optional<T>` 반환으로 NULL 안전성 확보
- `std::unique_ptr`로 런타임 저장소 교체 (의존성 주입)
- 임시 파일(`.tmp`) 기반 원자적 저장

---

## 네이밍 컨벤션

| 대상 | 규칙 | 예시 |
|------|------|------|
| 클래스 / 구조체 / 인터페이스 | PascalCase | `OrderService`, `IRepository` |
| 함수 / 메서드 | camelCase | `placeOrder()`, `getTotal()` |
| 멤버 변수 | camelCase | `orderId`, `price` |
| 지역 변수 / 매개변수 | camelCase | `itemCount`, `userId` |
| 상수 / `constexpr` | UPPER_SNAKE_CASE | `MAX_ORDER_COUNT` |
| `enum class` 타입 / 값 | PascalCase | `enum class OrderStatus { Pending }` |
| 파일명 | PascalCase | `OrderService.h`, `OrderService.cpp` |

---

## 커밋 규칙

커밋 메시지는 `[Phase N-N] 한 줄 설명` 형식으로 작성한다.

```
[Phase 1-1] 프로젝트 디렉터리 구조 및 빈 뼈대 생성
[Phase 1-2] 공통 헤더 및 타입 별칭 정의
[Phase 2-1] Product 모델 클래스 구현
[Phase 2-2] Customer 모델 클래스 구현
```

- Phase 태그는 `tmp_docs/plan.md`의 세부 Phase 번호와 일치시킨다.
- 설명은 구현 내용을 한 줄로 요약한다.
- 하나의 커밋은 하나의 세부 Phase 작업 단위를 원칙으로 한다.

---

## 빌드 규칙

- Debug / Release 모두 빌드 경고 0건 유지
- 원시 포인터 (`new` / `delete`) 직접 사용 금지 → 스마트 포인터 사용
- `nullptr` 사용 (`NULL`, `0` 금지)
