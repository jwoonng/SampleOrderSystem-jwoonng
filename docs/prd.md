# PRD — 반도체 시료 생산주문관리 시스템

**버전**: v1.0  
**작성일**: 2026-06-12

---

## 1. 사용자 역할

| 역할 | 설명 |
|------|------|
| **주문 담당자** | 고객 시료 요청을 접수하여 주문서 작성 |
| **생산 담당자** | 개발 시료 등록, 주문 수신 후 승인 또는 거절 |

---

## 2. 주문 상태 흐름

```
[주문 담당자]
    주문 등록 → RESERVED
                    │
             [생산 담당자] 승인 여부
            ┌───────┴───────┐
          거절             승인
            │               │
        REJECTED        재고 확인
                    ┌───────┴───────┐
                 재고 충분        재고 부족
                    │               │
               CONFIRMED       생산 라인 등록
               (출고 대기)      PRODUCING
                    │               │
                    │          생산 완료
                    │               │
                    └───────┬───────┘
                        CONFIRMED
                            │
                        출고 처리
                            │
                         RELEASE
```

| 상태 | 의미 |
|------|------|
| `RESERVED` | 주문 접수 |
| `REJECTED` | 주문 거절 (정상 흐름 외, 모니터링 제외) |
| `PRODUCING` | 승인 완료 + 재고 부족으로 생산 중 |
| `CONFIRMED` | 승인 완료 + 출고 대기 중 |
| `RELEASE` | 출고 완료 |

---

## 3. 기능 요구사항 체크리스트

| # | 기능 | 세부 요구사항 | 상태 |
|---|------|--------------|------|
| 3-1 | 메인 메뉴 | [feature/prd_001.md](feature/prd_001.md) | [ ] |
| 3-2 | 시료 관리 | [feature/prd_002.md](feature/prd_002.md) | [ ] |
| 3-3 | 시료 주문 | [feature/prd_003.md](feature/prd_003.md) | [ ] |
| 3-4 | 주문 승인/거절 | [feature/prd_004.md](feature/prd_004.md) | [ ] |
| 3-5 | 모니터링 | [feature/prd_005.md](feature/prd_005.md) | [ ] |
| 3-6 | 생산라인 | [feature/prd_006.md](feature/prd_006.md) | [ ] |
| 3-7 | 출고 처리 | [feature/prd_007.md](feature/prd_007.md) | [ ] |
| 3-8 | 실시간 생산 처리 | [feature/prd_008.md](feature/prd_008.md) | [ ] |

---

## 4. 비기능 요구사항

| 항목 | 내용 |
|------|------|
| 실행 환경 | Windows 콘솔 애플리케이션 |
| 언어 / 표준 | C++20 |
| 빌드 | Visual Studio 2022, MSBuild |
| 인코딩 | UTF-8 with BOM, 콘솔 wide character 모드 |
| 아키텍처 | MVC 계층 분리, Repository 패턴 |
| 데이터 저장 | 데이터 영속성 지원 (재실행 후에도 데이터 유지) |
| 개발 방식 | TDD (Red → Green → Verifying), Clean Code, SRP |

---

## 5. 변경 이력

| 날짜 | 버전 | 내용 |
|------|------|------|
| 2026-06-12 | v1.0 | PDF 기반 초안 작성 |
