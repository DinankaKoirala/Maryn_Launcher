import { Link } from 'react-router-dom'

export default function Home() {
  return (
    <main className="min-h-screen bg-zinc-950 text-white">
      <section className="px-8 py-24 text-center">
        <h1 className="text-5xl font-bold">
          Maryn Launcher
        </h1>

        <p className="mt-6 text-lg text-zinc-400">
          A simple and modern Minecraft launcher.
        </p>

        <div className="mt-8 flex justify-center gap-4">
          <Link to="/download" className="bg-white text-black px-6 py-3 rounded-lg font-semibold hover:bg-zinc-200 transition-colors">
            Download
          </Link>
          

          <Link to="/community" className="border border-zinc-700 px-6 py-3 rounded-lg hover:bg-zinc-900 transition-colors">
            Learn More
          </Link>
        </div>
      </section>
    </main>
  )
}