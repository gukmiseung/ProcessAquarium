# 팀 개발 안내

## 1. 시작

Windows PowerShell에서는 먼저 `wsl -d Ubuntu-24.04` (설치 이름은 wsl -l -v로 확인). Ubuntu에서 프로젝트를 홈 디렉터리에 복제하는 것을 권장합니다.

```bash
cd ~
git clone https://github.com/gukmiseung/ProcessAquarium.git
cd ProcessAquarium
sudo apt update
sudo apt install build-essential libncurses-dev git cloc
git fetch origin
```

각자 자신의 브랜치 하나만 선택합니다.

```bash
git switch feature/ui
# 또는 git switch feature/scheduler
# 또는 git switch feature/monitor
```

VS Code를 사용하면 Ubuntu 터미널에서 `code .`로 WSL 작업 공간을 열 수 있습니다(WSL 확장 필요).

## 2. 담당 범위

### A: UI

- [ ] ncurses 레이아웃, 물고기 애니메이션, 상태 색상(R/S/D/T/Z 등)
- [ ] PID/PPID 관계 및 시스템 자원/상세 정보 표시
- [ ] 작업 목록, 등록, 일시정지, 재개, 종료 입력 (job_id 사용)
- [ ] 선택 작업의 stdout/stderr 콘솔, 스크롤, 터미널 크기 대응
- [ ] make test-ui로 mock_backend만 사용해 독립 테스트

UI는 모니터 및 스케줄러 내부 자료구조에 직접 접근하지 않습니다. ncurses는 메인 스레드에서만 호출합니다. 실제 로그는 제어 문자(ESC 등)를 안전하게 표시하고 출력 길이를 제한합니다. 작업 등록은 argv 단위로 넘기며 셸 실행은 하지 않습니다.

### B: Scheduler & Output

- [ ] 우선순위 큐(낮은 숫자가 높은 우선순위, 동순위 FIFO), 동시 실행 슬롯 3개
- [ ] scheduler worker 및 condition variable, mutex, 작업 ID/상태/종료 결과
- [ ] fork/exec 또는 posix_spawnp, waitpid(WNOHANG 등), kill 기반 제어
- [ ] stdout/stderr 각각 pipe + dup2, 읽기 끝 O_NONBLOCK, poll worker
- [ ] 작업별 bounded log buffer, EOF/HUP/EINTR 처리, 종료 후 남은 출력 수집
- [ ] 대기 작업 취소, 소유 작업만 제어, 종료 시 자식 회수 및 FD 정리
- [ ] make test-scheduler로 UI 없이 테스트 (출력/제어/동시 실행 등 검증 추가)

실행 중인 임의의 외부 프로세스에는 출력 캡처 또는 제어를 적용하지 않습니다. 멀티스레드 fork 후 자식은 async-signal-safe 호출만 사용하고 execve/_exit 또는 posix_spawnp를 고려합니다. 출력 스레드와 스케줄러의 FD 소유권 및 종료 순서를 명확히 정합니다. SIGSTOP된 작업의 종료 처리, 자식의 PID 재사용 및 자식 프로세스 그룹 정리도 검토합니다.

### C: Monitor & Integration

- [ ] /proc 디렉터리 탐색, stat/status 및 meminfo 파싱
- [ ] 프로세스 CPU 변화량, RSS, 시스템 CPU/MEM/SWAP 계산
- [ ] PID/PPID/starttime 기반 생성·종료·재부모화 추적, 상태 표시용 데이터
- [ ] monitor pthread 주기적 수집(예: 0.5초), mutex로 스냅샷 교체 및 복사
- [ ] monitor_start/stop에서 스레드 생성 및 join, main에서 모듈 생명주기 관리
- [ ] make test-monitor로 UI 없이 테스트, ps/top/free와 단위 및 상태 비교
- [ ] 통합 빌드/Makefile 관리, 최종 Ubuntu 테스트

/proc 수집 중 사라진 프로세스, 접근 권한, stat의 괄호 포함 이름을 처리합니다. 프로세스 정체성은 PID와 start_time을 함께 사용합니다. 초기 CPU 측정은 두 샘플이 필요합니다.

## 3. 공통 인터페이스

include/common.h는 공통 타입, 나머지 헤더는 모듈 간 약속입니다. 공통 타입/공개 함수 변경은 팀 합의 후 별도 PR로 진행합니다.

- 모든 조회는 호출자 소유 버퍼에 복사합니다. 반환값은 복사한 개수(0 포함) 또는 음수 errno입니다. system info는 성공 0입니다.
- 내부 데이터와 mutex는 각 담당 모듈이 소유합니다. 잠금 중 I/O, UI 렌더링 및 다른 모듈 호출은 피합니다.
- scheduler_add_job은 양수 job_id 또는 음수 errno를 반환합니다. argv는 함수 반환 전에 복사합니다.
- 제어 함수는 0이면 요청 접수, 음수면 실패. 실제 상태는 후속 조회로 확인합니다.
- OutputLog는 job_id별 최근 기록의 복사본입니다. 순서는 오래된 것부터, sequence는 증가값입니다. 출력은 부분 줄일 수 있고 보관량은 제한합니다.
- Linux 프로세스 상태와 스케줄러 JobStatus는 구별합니다. UI가 매핑하며 queued 작업 PID는 0입니다.
- 모니터는 시스템 전체를 관찰하고, 스케줄러는 직접 등록/실행한 작업만 제어합니다.
- 스레드 구조 목표: main/UI, monitor worker, scheduler worker, output poll worker. 초기 스텁에는 실제 스레드가 아직 없습니다.

## 4. Git 작업

자기 브랜치에서만 수정 및 커밋합니다.

```bash
git status
git add <내가 수정한 파일>
git commit -m "Implement my module"
git push
```

GitHub에서 자신의 브랜치 -> develop으로 Pull Request를 생성합니다. 다른 브랜치 기능이 develop에 들어오면 작업 브랜치에 반영합니다.

```bash
git fetch origin
git merge origin/develop
```

충돌이 나면 담당자와 확인 후 해결하고, 다른 사람 파일을 임의로 덮어쓰지 않습니다. develop에서 기능별 연동 검증 후 main에 PR을 생성합니다. 새로운 소스 파일을 추가하면 C 담당과 Makefile 목록을 맞춥니다.

## 5. 통합 체크

- [ ] make 및 make test (실제 검증 로직 보강 후)
- [ ] 실제 프로세스 목록과 자원 정보 표시
- [ ] 작업 등록 → 우선순위 실행 → 출력 콘솔 → 종료 코드 확인
- [ ] 여러 작업의 stdout/stderr 동시 출력, 일시정지/재개/종료
- [ ] UI 응답성 유지, 오류/권한 및 종료 처리, FD/자식 프로세스 누수 없음
- [ ] cloc 결과와 실제 시스템 콜, 독립 핵심 기능 수 정리

가이드라인에 따라 최종 구현은 팀이 직접 이해하고 작성/검증하며 기여도를 명확히 기록합니다.
